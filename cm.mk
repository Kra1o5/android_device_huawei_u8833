# Specify phone tech before including full_phone
$(call inherit-product, vendor/cm/config/gsm.mk)

# Inherit some common CM stuff.
$(call inherit-product, vendor/cm/config/common_full_phone.mk)

# Inherit device configuration
$(call inherit-product, device/huawei/u8833/u8833.mk)

# Correct boot animation size for the screen.
TARGET_BOOTANIMATION_NAME := vertical-480x800

# Device identifier. This must come after all inclusions
PRODUCT_DEVICE := u8833
PRODUCT_NAME := cm_u8833
PRODUCT_BRAND := Huawei
PRODUCT_MODEL := Ascend Y300
PRODUCT_MANUFACTURER := HUAWEI
PRODUCT_RELEASE_NAME := u8833
