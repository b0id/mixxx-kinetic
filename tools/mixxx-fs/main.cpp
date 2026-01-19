#include <fuse_lowlevel.h>

#include <QCoreApplication>
#include <QDebug>
#include <QTimer>
#include <csignal>
#include <cstring>
#include <iostream>
#include <memory>
#include <thread>

#include "streaming/bridge/bridgeserver.h"
#include "streaming/bridge/fusedriver.h"

static struct fuse_session* se;

static void handle_signal(int signum) {
    (void)signum;
    if (se != NULL)
        fuse_session_exit(se);
    QCoreApplication::quit();
}

static struct fuse_lowlevel_ops kinetic_oper = {
        .init = FuseDriver::init,
        .lookup = FuseDriver::lookup,
        .getattr = FuseDriver::getattr,
        .open = FuseDriver::open,
        .read = FuseDriver::read,
};

int main(int argc, char* argv[]) {
    // Initialize QCoreApplication
    QCoreApplication app(argc, argv);

    // Initialize services early to avoid goto scope issues
    auto driver = std::make_unique<FuseDriver>();
    auto bridgeServer = std::make_unique<BridgeServer>();

    struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
    struct fuse_cmdline_opts opts;
    int ret = -1;

    if (fuse_parse_cmdline(&args, &opts) != 0)
        return 1;

    if (opts.show_help) {
        printf("usage: %s [options] <mountpoint>\n\n", argv[0]);
        fuse_cmdline_help();
        fuse_lowlevel_help();
        ret = 0;
        goto err_out1;
    } else if (opts.show_version) {
        printf("FUSE library version %s\n", fuse_pkgversion());
        fuse_lowlevel_version();
        ret = 0;
        goto err_out1;
    }

    if (opts.mountpoint == NULL) {
        printf("usage: %s [options] <mountpoint>\n", argv[0]);
        printf("       %s --help\n", argv[0]);
        ret = 1;
        goto err_out1;
    }

    // FuseDriver singleton created at top of main

    se = fuse_session_new(&args, &kinetic_oper, sizeof(kinetic_oper), NULL);
    if (se == NULL)
        goto err_out1;

    if (fuse_set_signal_handlers(se) != 0)
        goto err_out2;

    if (fuse_session_mount(se, opts.mountpoint) != 0)
        goto err_out3;

    fuse_daemonize(opts.foreground);

    // BridgeServer initialized at top of main
    if (!bridgeServer->listen()) {
        std::cerr << "Failed to start Bridge IPC Server!" << std::endl;
        // Proceed anyway? Or fail? Fail for now.
        // goto err_unmount; // Handle cleanup
    }

    std::cout << "Starting Mixxx-Kinetic FUSE Loop on " << opts.mountpoint << "..." << std::endl;
    // ret = fuse_session_loop(se);
    // We cannot use standard fuse_session_loop because we need to process Qt events too.
    // FUSE needs to read from the FUSE file descriptor.

    // Option 1: Run FUSE loop in a separate thread.
    // Option 2: Integrate FUSE fd into Qt event loop via QSocketNotifier.

    // Let's use Option 1 for simplicity of implementation for now, assuming thread safety in FuseDriver was added.
    // Actually, `fuse_session_loop` blocks. We can run it in a std::thread.
    // Caveat: signals need to be handled carefully.

    {
        std::thread fuseThread([&]() {
            fuse_session_loop(se);
            // When loop exits (due to unmount or signal), quit app
            QCoreApplication::quit();
        });

        // Run Qt Event Loop (for IPC)
        ret = app.exec();

        // Ensure fuse thread stops if app quits first?
        fuse_session_exit(se);
        if (fuseThread.joinable()) {
            fuseThread.join();
        }
    }

    fuse_session_unmount(se);
err_out3:
    fuse_remove_signal_handlers(se);
err_out2:
    fuse_session_destroy(se);
err_out1:
    free(opts.mountpoint);
    fuse_opt_free_args(&args);

    return ret ? 1 : 0;
}
