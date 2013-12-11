CyanogenMod device configuration for the Huawei Ascend Y300/G510.

How to Build
---------------

Initialise from CyanogenMod:

    repo init -u git://github.com/CyanogenMod/android.git -b jellybean

Use the following local manifest:

    <?xml version="1.0" encoding="UTF-8"?>
    <manifest>
    <project name="Kra1o5/android_device_huawei_u8833" path="device/huawei/u8833" revision="jellybean" />
    <project name="Kra1o5/android_kernel_huawei_msm8x25-common" path="kernel/huawei/u8833" revision="jb" />
    <project name="Kra1o5/proprietary_vendor_huawei" path="vendor/huawei" revision="jellybean" />

    <!-- FM Radio -->
    <project path="hardware/qcom/fm" name="legaCyMod/android_hardware_qcom_fm" revision="jellybean" />
    <project path="packages/apps/FM2" name="legaCyMod/android_packages_apps_FM2" revision="jellybean" />
    </manifest>

Sync and build:

    repo sync -j4
    . build/envsetup.sh
    brunch u8833
