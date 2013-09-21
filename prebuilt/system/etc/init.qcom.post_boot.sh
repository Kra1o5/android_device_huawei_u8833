#!/system/bin/sh
# Copyright (c) 2009-2012, Code Aurora Forum. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of Code Aurora nor
#       the names of its contributors may be used to endorse or promote
#       products derived from this software without specific prior written
#       permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NON-INFRINGEMENT ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

target=`getprop ro.board.platform`
        echo "hyper" > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor
        echo "hyper" > /sys/devices/system/cpu/cpu1/cpufreq/scaling_governor
     chown system /sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq
	 chown system /sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq
	 chown system /sys/devices/system/cpu/cpu1/cpufreq/scaling_max_freq
	 chown system /sys/devices/system/cpu/cpu1/cpufreq/scaling_min_freq
	 chown root.system /sys/devices/system/cpu/cpu1/online
	 chmod 664 /sys/devices/system/cpu/cpu1/online

# Enable Power modes 
	 start qosmgrd
	echo 1 > /sys/module/pm2/modes/cpu0/standalone_power_collapse/idle_enabled
	echo 1 > /sys/module/pm2/modes/cpu1/standalone_power_collapse/idle_enabled
	echo 1 > /sys/module/pm2/modes/cpu0/standalone_power_collapse/suspend_enabled
	echo 1 > /sys/module/pm2/modes/cpu1/standalone_power_collapse/suspend_enabled
	#SuspendPC:
	echo 1 > /sys/module/pm2/modes/cpu0/power_collapse/suspend_enabled
	#IdlePC:
	echo 1 > /sys/module/pm2/modes/cpu0/power_collapse/idle_enabled

	echo 10 > /sys/devices/platform/msm_sdcc.1/idle_timeout
# Change adj level and min_free_kbytes setting for lowmemory killer to kick in

	echo 0,1,2,4,9,12 > /sys/module/lowmemorykiller/parameters/adj
	echo 5120 > /proc/sys/vm/min_free_kbytes
	
        chown system /sys/devices/platform/rs300000a7.65536/force_sync
        chown system /sys/devices/platform/rs300000a7.65536/sync_sts
        chown system /sys/devices/platform/rs300100a7.65536/force_sync
        chown system /sys/devices/platform/rs300100a7.65536/sync_sts

# Post-setup services
	soc_id=`cat /sys/devices/system/soc/soc0/id`
    ver=`cat /sys/devices/system/soc/soc0/version`
        case "$soc_id" in
            "127" | "128" | "129" | "137")
                start mpdecision
                if [ "$ver" = "2.0" ]; then
                        start thermald
                fi
        ;;
        esac


# Install AdrenoTest.apk if not already installed
if [ -f /data/prebuilt/AdrenoTest.apk ]; then
    if [ ! -d /data/data/com.qualcomm.adrenotest ]; then
        pm install /data/prebuilt/AdrenoTest.apk
    fi
fi
