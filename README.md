CyanogenMod device configuration for the Huawei Ascend Y300/G510.

How to Build
---------------

Initialise from CyanogenMod:

    repo init -u git://github.com/CyanogenMod/android.git -b cm-10.1

Use the following local manifest:

    <?xml version="1.0" encoding="UTF-8"?>
    <manifest>
    <remove-project name="CyanogenMod/android_frameworks_av" />
    <remove-project name="CyanogenMod/android_frameworks_native" />
    <remove-project name="CyanogenMod/android_hardware_qcom_media-caf" />
    <project path="hardware/atheros/wlan" name="legaCyMod/android_hardware_atheros_wlan" revision="cm-10.1" />
    <project path="frameworks/av" name="Kra1o5/android_frameworks_av" revision="cm-10.1" />
    <project path="frameworks/native" name="Kra1o5/android_frameworks_native" revision="cm-10.1" />
    <project name="Kra1o5/android_device_huawei_u8833" path="device/huawei/u8833" revision="cm-10.1" />
    <project name="Kra1o5/android_kernel_huawei_msm8x25-common" path="kernel/huawei/u8833" revision="jb" />
    <project name="Kra1o5/proprietary_vendor_huawei" path="vendor/huawei" revision="cm-10.1" />
    <project name="Kra1o5/android_hardware_qcom_display-legacy" path="hardware/qcom/display-legacy" revision="cm-10.1" />
    <project name="Kra1o5/android_hardware_qcom_media-caf" path="hardware/qcom/media-caf" revision="cm-10.1" />

    <!-- FM Radio -->
    <project path="hardware/qcom/fm" name="legaCyMod/android_hardware_qcom_fm" revision="cm-10.1" />
    <project path="packages/apps/FM2" name="legaCyMod/android_packages_apps_FM2" revision="cm-10.1" />
    </manifest>

Sync and build:

    repo sync -j4
    . build/envsetup.sh
    brunch u8833

