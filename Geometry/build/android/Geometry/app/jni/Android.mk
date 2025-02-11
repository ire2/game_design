# Top level NDK Makefile for Android application
# Module settings for CUGL
SUPPORT_AUDIO := false
SUPPORT_SCENE2 := true
SUPPORT_SCENE3 := false
SUPPORT_PHYSICS2 := true
SUPPORT_NETCODE  := false
SUPPORT_DISTRIB_PHYSICS2 := false

# We just invoke the subdirectories
include $(call all-subdir-makefiles)
