[33mcommit 9ce83e23b5d3473664999e65841d36ee8842f3fc[m[33m ([m[1;36mHEAD -> [m[1;32mmaster[m[33m, [m[1;31morigin/master[m[33m, [m[1;31morigin/HEAD[m[33m)[m
Author: shossains <shaansona@hotmail.com>
Date:   Fri Jul 5 16:59:36 2024 +0200

    add gsl

[33mcommit 8ba0ec0defd35fb380b50dfe37342ea145db6693[m
Author: shossains <shaansona@hotmail.com>
Date:   Thu Jul 4 01:24:25 2024 +0200

    GSL

[33mcommit ebeedc6a58b7eedbd6c0d0f0c9682f2abf26b7e2[m
Author: shossains <shaansona@hotmail.com>
Date:   Mon Jun 24 21:02:23 2024 +0200

    add fc files

[33mcommit 5886144aef13b92a0f26b971c090b49f5fe2d00d[m
Author: shossains <shaansona@hotmail.com>
Date:   Mon Jun 24 20:40:46 2024 +0200

    disable startup sound

[33mcommit 7cbe7cc56ff52d9b0b7f9442858d801eca35a6bd[m
Merge: d8fd72fc 602216d8
Author: Rik <49898887+gemenerik@users.noreply.github.com>
Date:   Thu Jun 20 11:13:22 2024 +0200

    Merge pull request #1385 from sojez1/patch-1
    
    Correct cfloader flash command in build.md

[33mcommit 602216d84a330beb7b0822885f2554b41e2d859d[m
Author: Jean-Pierre S <48832225+sojez1@users.noreply.github.com>
Date:   Wed Jun 19 11:29:35 2024 -0400

    Update build.md
    
    Flashing crazyflie - Automatically enter boatloader
    
    old: cfloader flash cf2.bin stm32-fw -w [CRAZYFLIE_URI]
    new: cfloader flash build/cf2.bin stm32-fw -w [CRAZYFLIE_URI]
    
    the cf2.bin file is located inside the build folder

[33mcommit d8fd72fcb7ccff95d3f2e7a03d2f40c289f6d982[m
Merge: fe48d59b aa03faad
Author: Tove <tove@bitcraze.io>
Date:   Tue Jun 11 16:08:02 2024 +0200

    Merge pull request #1384 from bitcraze/toverumar/mutual_excl_idlethrust
    
    Toverumar/mutual excl idlethrust

[33mcommit aa03faad1a96e0c0ccb31261074cc308dca4f9bc[m
Author: Tove Rumar <tove@bitcraze.io>
Date:   Tue Jun 11 11:04:54 2024 +0200

    Block idlethtrust and unarmed feature at run time.
    
    We need to override the idleThrust param stored in eeprom with 0 if arming is not on
    The (brushless) motors needs to be fed a 0 signal after they started up and we do not know exactly when that is.
    These two features are mutually eclusive and can not be used together

[33mcommit 7dbccfd59aa0dff47a65ca660c17c71afacf820f[m
Author: Tove Rumar <tove@bitcraze.io>
Date:   Tue Jun 11 10:15:12 2024 +0200

    Stop building a FW that has both arming set to off, and idle_thrust higher than 0.
    
    Will only stop build time misconfiguration

[33mcommit fe48d59b8246c70ebac0c2c371eb0dcd8347f9e6[m
Merge: b15eebc8 e87e2bad
Author: Tobias Antonsson <tobbeanton@gmail.com>
Date:   Tue Jun 11 10:37:24 2024 +0200

    Merge pull request #1382 from bitcraze/toverumar/add_bl_test_deck
    
    Add RPM deck test for Brushless

[33mcommit e87e2bad6f963e6e1f990827811dd878969ac4ca[m
Author: Tove Rumar <tove@bitcraze.io>
Date:   Mon Jun 10 16:17:53 2024 +0200

    Add all rpm testing logic to exptest driver for BL

[33mcommit b15eebc864e7e734daa3766d116fa26edbc940a9[m
Merge: a7c91571 c9ec44cd
Author: Tobias Antonsson <tobbeanton@gmail.com>
Date:   Mon Jun 10 15:34:20 2024 +0200

    Merge pull request #1383 from bitcraze/rik/blplatform
    
    Add platform configuration for CF2.1 brushless

[33mcommit 2e36e3dafadec400be9b12e35d9ff4a3e698146b[m
Author: Tove Rumar <tove@bitcraze.io>
Date:   Mon Jun 10 15:25:58 2024 +0200

    Rename exptestCfBL driver

[33mcommit c9ec44cdebdbbce0a606c07dfe9dfeb4b1ffb111[m
Author: Rik Bouwmeester <rikjbouwmeester@gmail.com>
Date:   Wed Jun 5 11:26:33 2024 +0200

    Remove unused sensor drivers for the CF21BL

[33mcommit a7c91571fed853122ecc22e3cb5756660ba166e8[m
Merge: 46c13878 c2b94970
Author: Rik <49898887+gemenerik@users.noreply.github.com>
Date:   Tue Jun 4 14:55:35 2024 +0200

    Merge pull request #1381 from bitcraze/rik/otafix
    
    Fix multi-deck multi-memory OTA flashing

[33mcommit ebbc5bd91f99bf502c2d51538b2254b4c7320852[m
Author: Rik Bouwmeester <rikjbouwmeester@gmail.com>
Date:   Tue Jun 4 14:47:01 2024 +0200

    Force manual arming for brushless
    
    This automatically forces manual arming if users manually select the brushless as the platform in kbuild instead of by loading the brushless defconfig.
    Without this, users could compile a flyable firmware and accidentally forget to enable manual arming.
    People that want autoarming on the brushless platform will have to modify the kbuild system.

[33mcommit 7e0ccce179d327cc16fb3d7896f645c737d71b51[m
Merge: 35f5092a 46c13878
Author: Rik Bouwmeester <rikjbouwmeester@gmail.com>
Date:   Tue Jun 4 13:55:55 2024 +0200

    Merge branch 'master' into rik/blplatform

[33mcommit 35f5092a5dd985f4a77a676466c20d18535e7a41[m
Author: Rik Bouwmeester <rikjbouwmeester@gmail.com>
Date:   Tue Jun 4 13:53:59 2024 +0200

    Add platform configuration for CF2.1 brushless
    
    - Platform comes with modified defaults for PID tuning, idle thrust, platform mass, and battery limit compared to CF2.1.
    - The brushless defconfig enables manual arming and the DSHOT ESC protocol.
    - Moved idle thrust configuration out of kbuild.

[33mcommit 46c1387834ef546c83715263c52c7280353134c7[m
Merge: e6c41593 5d9493f1
Author: Arnaud Taffanel <arnaud@bitcraze.io>
Date:   Mon Jun 3 10:46:19 2024 +0200

    Merge pull request #1374 from jpreiss/param_warning
    
    fix narrowing warning for param definitions

[33mcommit e6c415933adc657aec080763bb95622cd0b51b0e[m
Merge: 4a359979 3c645883
Author: Arnaud Taffanel <arnaud@bitcraze.io>
Date:   Mon Jun 3 10:43:42 2024 +0200

    Merge pull request #1379 from IMRCLab/feature_print_nan
    
    printf: add support to print NaNs

[33mcommit 4a3599797814be3914ebab58726405d7a7f65c57[m
Merge: a25e8fb2 77d9afbe
Author: Kimberly McGuire <kimberly@bitcraze.io>
Date:   Fri May 31 11:01:25 2024 +0200

    Merge pull request #1378 from IMRCLab/bugfix_lee_order
    
    Lee controller: fix inconsistend order h/c file

[33mcommit 575f7bc5abd02fcca4b3083761aa0b4091c64735[m
Author: Tove Rumar <tove@bitcraze.io>
Date:   Wed May 29 14:29:06 2024 +0200

    Modify rpm deck driver test to work properly on Brushless motors
    
    The ESCs needs to be fed a signal constantly to not go to sleep,
    however we can not start the test until ESCs are started (which we dont
    know exactly when that is).
    
    To get around this issue we will wait a time, while constantly feeding the
    ESCs at 1KHz with a 0 PWM signal. After this time we start the test.

[33mcommit 3c6458832728c5cca104b1a3d136de2d6bfa5116[m
Author: Wolfgang Hoenig <hoenig@tu-berlin.de>
Date:   Wed May 29 21:27:06 2024 +0200

    printf: add support to print NaNs

[33mcommit 77d9afbe14f4f67072b415fe720638464751b81d[m
Author: Wolfgang Hoenig <hoenig@tu-berlin.de>
Date:   Wed May 29 21:24:19 2024 +0200

    Lee controller: fix inconsisten order h/c file
    
    * h file had #ifdef CONFIG_CONTROLLER_OOT after Lee; c-file had it before
    * Add define to force usage of Lee controller

[33mcommit c2b949702e2a64017f769ad52f39c952359b5cf7[m
Author: Rik Bouwmeester <rikjbouwmeester@gmail.com>
Date:   Wed May 29 15:49:19 2024 +0200

    Fix multi-deck multi-memory OTA flashing

[33mcommit 2d8c67a93c70546288281defc1b92a900951b3f9[m
Author: Tove Rumar <tove@bitcraze.io>
Date:   Wed May 22 14:28:19 2024 +0200

    Run all motors in parallell instead

[33mcommit dc961e7a9d2385ff75eef4fcdea786548f8f4848[m
Author: Tove Rumar <tove@bitcraze.io>
Date:   Wed May 22 14:15:49 2024 +0200

    Tighten pass criteria and log the data

[33mcommit d27e2ed14c1a0faedeb9d98739e2a4b25aa4767c[m
Author: Tove Rumar <tove@bitcraze.io>
Date:   Wed May 22 13:59:23 2024 +0200

    Remove pins connected to reflector sensors from pin test.
    
    These pins will be tested indirectly from the rpm test by reading on them.

[33mcommit e64fa9f4e79968dc3ca06e3f47deb4095d3eb7e4[m
Author: Tove Rumar <tove@bitcraze.io>
Date:   Wed May 22 13:40:43 2024 +0200

    Remove vscode settings from repo
    
    Keep a local copy instead

[33mcommit 9b4c68165cc6ad51e14afa6eee8c60ba1081fce5[m
Author: Tove Rumar <tove@bitcraze.io>
Date:   Wed May 22 13:07:33 2024 +0200

    Add a propeller  RPM test to the rpm.c driver.
    
    The limits are not yet decided
    The test will run for approx 30 sec, and will feed the ESCs with a new PWM signal every 50 msec to prevent it from going asleep.
    Run one test per motor one at a time.

[33mcommit 81b7ac99345a4b876f050349aa022ddd9e8119c4[m
Author: Tove Rumar <tove@bitcraze.io>
Date:   Wed May 22 12:53:01 2024 +0200

    RPM deck driver should be compiled by default on all platforms
    
    It is needed for production on CF-BL but it is not any issue of inluding it on other platforms aswell

[33mcommit 7e43df14fe78466dee2d67fdba21bfcc3ffd0fea[m
Author: Tove Rumar <tove@bitcraze.io>
Date:   Tue May 21 15:04:30 2024 +0200

    Create a driver for the test deck for Brushless

[33mcommit a25e8fb2254f6d5be54ad57566b3b846a97de93b[m
Merge: d9ee34a7 82682361
Author: Kimberly McGuire <kimberly@bitcraze.io>
Date:   Thu May 16 13:16:35 2024 +0200

    Merge pull request #1343 from IMRCLab/upstream_lee
    
    Add Lee controller

[33mcommit 5d9493f1b63fdd55b7ab8494d558f74708d16375[m
Author: jpreiss <jamespalanpreiss@gmail.com>
Date:   Sat May 4 13:36:29 2024 -0700

    silence clang narrowing warning for bindings

[33mcommit d9ee34a78cecf2eaf8b05e0b54487cff0c7b4590[m
Merge: c2ddfe56 d358f59e
Author: Tobias Antonsson <tobbeanton@gmail.com>
Date:   Wed Apr 24 15:50:02 2024 +0200

    Merge pull request #1371 from bitcraze/fix-auto-doc
    
    autogen log and param fixes

[33mcommit d358f59e89ee622142b0b626605c929ccdadd118[m
Author: knmcguire <kimberly@bitcraze.io>
Date:   Wed Apr 24 14:52:17 2024 +0200

    add doxygen ignore line conditions

[33mcommit 37fbdcd4458a0998cd4f4678674952de61464706[m
Author: knmcguire <kimberly@bitcraze.io>
Date:   Wed Apr 24 14:41:51 2024 +0200

    remove defines of twr - standard is 8 now

[33mcommit c2ddfe56cd1016e3182088eba8d7799a3d4c94ca[m
Merge: 9fe3099f c945c7c9
Author: Tove Rumar <tove@bitcraze.io>
Date:   Mon Apr 22 14:40:11 2024 +0200

    Merge pull request #1366 from bitcraze/toverumar/update_unit_docs
    
    Documentation: Add instructions for workaround for deadlysignal in un…

[33mcommit 9fe3099f8e2f9560d3dbb7d9e94c70f07edc7aee[m
Merge: 9bcb278a 9faaa4da
Author: Arnaud Taffanel <arnaud@bitcraze.io>
Date:   Mon Apr 15 13:37:15 2024 +0200

    Merge pull request #1364 from bitcraze/ToveRumar/defrag_kve_on_startup
    
    Storage; Defragment kve storage when starting up by default

[33mcommit c945c7c93416c62dd3696c3207064344c8868935[m
Author: Tove Rumar <tove@bitcraze.io>
Date:   Fri Apr 12 11:37:52 2024 +0200

    Documentation: Add instructions for workaround for deadlysignal in unit tests.
    
    There is a incompatibility between address sanitizer and recent ubuntu versions that causes
    an eternal loop of deadly signal when running unit tests in local environment.
    See this issue for more info https://github.com/google/sanitizers/issues/1716

[33mcommit 9bcb278a03f1e659e4a42f28a91a43def8098757[m
Merge: 28e647bc e5000188
Author: Tove Rumar <tove@bitcraze.io>
Date:   Thu Apr 11 14:53:59 2024 +0200

    Merge pull request #1365 from bitcraze/toverumar/move_svd_debug_file
    
    STM32F405.svd: Move the svd file for debugging.

[33mcommit e500018816df60cf88ac36a824fac11fd951d263[m
Author: Tove Rumar <tove@bitcraze.io>
Date:   Thu Apr 11 14:18:21 2024 +0200

    STM32F405.svd: Move the svd file for debugging.
    
    Move the svd file for debugging to a tools-location.
    This way the user does not have to move this file around to debug.
    Also change this in the documentation

[33mcommit 9faaa4da5c19ce1efc5e89c5fe5bf86cd15800ab[m
Author: Tove Rumar <tove@bitcraze.io>
Date:   Thu Apr 11 10:19:37 2024 +0200

    Storage; Defragment kve storage when starting up by default
    
    The kve will be defragmented before running the kve sanity check.
    Defragmenting before checking will make sure that the kve is sane after defragmentation
    and if not the kve will be reformatted.
    
    This can be turned of in the Kconfig, so that users that do not want this to possibly
    change their startup time (if eeprom is severely fragmented) can go back to only defragmenting
    once kve is full

[33mcommit 28e647bc0043fe7aa737dc2cddecb06a9674c525[m
Merge: dbb09b5c ced8fdd6
Author: Tove Rumar <45783739+ToveRumar@users.noreply.github.com>
Date:   Thu Apr 11 08:46:05 2024 +0200

    Merge pull request #1363 from bitcraze/ToveRumar/unit_tests_update_for_ruby
    
    unit tests update for newer ruby versions

[33mcommit ced8fdd6f2bb435bc4fc58dee67c113653f6c4d2[m
Author: Tove Rumar <tove@bitcraze.io>
Date:   Tue Apr 9 15:55:09 2024 +0200

    UnitTests: Accomodate for change in Unity.

[33mcommit 7b1dc99f90bf1731a88e8696e055e693e039d9cc[m
Author: Tove Rumar <tove@bitcraze.io>
Date:   Tue Apr 9 15:46:06 2024 +0200

    rakefile_helper: Update yaml load api to helper from unity
    
    This is needed due to a breaking change in Ruby versions with Psych 4.0 or later

[33mcommit f80484576856564ebf6cc9874d7921366c0a9f8a[m
Author: Tove Rumar <tove@bitcraze.io>
Date:   Tue Apr 9 15:44:33 2024 +0200

    Unity: Update submodule ref to v2.6.0
    
    Update Unity submodule to include latest changes.
    This particular update is needed for a breaking change in Ruby versions whth Psych 4.0 and up

[33mcommit dbb09b5ca16f0ddf63e98d2c44d247a3aa15f056[m
Author: Tobias Antonson <tobbeanton@gmail.com>
Date:   Thu Apr 4 17:03:10 2024 +0200

    Added init reset of CF-BL ESCs

[33mcommit 1cf603dba202d5139d5933c71949e1a2d6c122fa[m
Merge: 880be297 b51c8356
Author: Tobias Antonson <tobbeanton@gmail.com>
Date:   Thu Apr 4 16:28:02 2024 +0200

    Merge branch 'master' of https://github.com/bitcraze/crazyflie-firmware

[33mcommit b51c8356e74aa994cc2464e4b3c071f7fc0a61b2[m
Merge: cca276bc 81decdf5
Author: Tobias Antonsson <tobbeanton@gmail.com>
Date:   Wed Apr 3 16:33:26 2024 +0200

    Merge pull request #1361 from bitcraze/rik/bmp390support
    
    Introduce BMP390 support

[33mcommit 81decdf5091560865e1fe0909dbe9f955c298704[m
Author: Rik Bouwmeester <rikjbouwmeester@gmail.com>
Date:   Wed Apr 3 16:09:03 2024 +0200

    Introduce BMP390 support
    
    No need to discern between them besides the printouts.

[33mcommit 880be2970eddb3306ecffc5939f16fce17f347e9[m
Merge: 2c311078 cca276bc
Author: Tobias Antonson <tobbeanton@gmail.com>
Date:   Thu Mar 28 10:05:52 2024 +0100

    Merge branch 'master' of https://github.com/bitcraze/crazyflie-firmware

[33mcommit cca276bc6e952307a0a81c475fc6856c776a4a1c[m
Merge: 3d5129ff bb6d852a
Author: Kimberly McGuire <kimberly@bitcraze.io>
Date:   Tue Mar 26 16:32:56 2024 +0100

    Merge pull request #1356 from bitcraze/knmcguire-patch-3
    
    Update openocd_gdb_debugging.md

[33mcommit bb6d852adbe2f9719e78e0bda5e3b679d5d7fca4[m
Author: Kimberly McGuire <kimberly@bitcraze.io>
Date:   Tue Mar 26 14:50:43 2024 +0100

    add .svd file

[33mcommit b5af5f1b728c56fc7abd677d221b547bacae4571[m
Author: Kimberly McGuire <kimberly@bitcraze.io>
Date:   Tue Mar 26 14:49:48 2024 +0100

    Update openocd_gdb_debugging.md

[33mcommit 3d5129ff6afbde9bfe12cc70cb0dfaf948423316[m
Merge: 1f48f754 1f48a88e
Author: Tobias Antonsson <tobbeanton@gmail.com>
Date:   Fri Mar 22 14:17:20 2024 +0100

    Merge pull request #1346 from bitcraze/dev-systemId
    
    Merge SystemID deck functionality into master

[33mcommit 1f48a88e5d1d54f79e0522bf18be8883bae44eb9[m
Author: Tobias Antonson <tobbeanton@gmail.com>
Date:   Fri Mar 22 12:33:30 2024 +0100

    Re-added cnfigblocktest and fixes from review

[33mcommit 1f48f7547fa7fb9ec43bfd76c3082395653cdcbe[m
Author: Marcus <marcus.c.eliasson@gmail.com>
Date:   Thu Mar 21 15:54:18 2024 +0100

    Updated supervisor state diagram in documentation

[33mcommit db3e0f0cf75ba48053dd408264c3323507e6d637[m
Author: Tobias Antonson <tobbeanton@gmail.com>
Date:   Wed Mar 20 13:52:03 2024 +0100

    updated submodule libdw1000 which was old

[33mcommit d72ee9a44e51bc1d3def0843cfe05ad7acc953bb[m
Merge: 046b638b f61da11d
Author: Tobias Antonson <tobbeanton@gmail.com>
Date:   Wed Mar 20 13:39:55 2024 +0100

    Merge branch 'master' of https://github.com/bitcraze/crazyflie-firmware into dev-systemId
