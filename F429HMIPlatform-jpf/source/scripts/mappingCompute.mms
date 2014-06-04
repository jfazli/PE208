# Copyright 2009-2013 IS2T. All rights reserved.

# This script remove symbols defined in several graphs in order that each symbol and section are defined
# in only one graph.

# "$Computed" graph contains all symbols that have already been added in another graph
union $Computed $Label_Strings $Label_LIBFloat
union $Computed $Computed $Label_VMDebugger
union $Computed $Computed $Label_VMConsole
union $Computed $Computed $Label_JavaStrings
union $Computed $Computed $Label_Statics

# CLDC_B-ON
substract $Label_CLDC_B-ON $Label_CLDC_B-ON $Computed
union $Computed $Computed $Label_CLDC_B-ON

# PAP
substract $Label_PAP $Label_PAP $Computed
union $Computed $Computed $Label_PAP

# ECOM
substract $Label_ECOM $Label_ECOM $Computed
union $Computed $Computed $Label_ECOM

# MicroUI
substract $Label_MicroUI $Label_MicroUI $Computed
union $Computed $Computed $Label_MicroUI

# Widgets
substract $Label_Widgets $Label_Widgets $Computed
union $Computed $Computed $Label_Widgets

# MWT
substract $Label_MWT $Label_MWT $Computed
union $Computed $Computed $Label_MWT

# OSGiME
substract $Label_OSGiME $Label_OSGiME $Computed
union $Computed $Computed $Label_OSGiME

# NLS
substract $Label_NLS $Label_NLS $Computed
union $Computed $Computed $Label_NLS

# SP
substract $Label_SP $Label_SP $Computed
union $Computed $Computed $Label_SP

# StoryBoard
substract $Label_StoryBoard $Label_StoryBoard $Computed
union $Computed $Computed $Label_StoryBoard

# Internet Pack
substract $Label_Internet_Pack $Label_Internet_Pack $Computed
union $Computed $Computed $Label_Internet_Pack

# Java Runtime
substract $Label_JavaRuntime $Label_JavaRuntime $Computed
union $Computed $Computed $Label_JavaRuntime

# DRIVER
substract $Label_Drivers $Label_Drivers $Computed
union $Computed $Computed $Label_Drivers 

# PNG
substract $Label_LIBPNG $Label_LIBPNG $Computed
union $Computed $Computed $Label_LIBPNG 

# BMPM
substract $Label_LIBBMPM $Label_LIBBMPM $Computed
union $Computed $Computed $Label_LIBBMPM

# DISPLAY
substract $Label_LIBDisplay $Label_LIBDisplay $Computed
union $Computed $Computed $Label_LIBDisplay 

# LIBInput
substract $Label_LIBInput $Label_LIBInput $Computed
union $Computed $Computed $Label_LIBInput 

# ICETEART
substract $Label_IceteaRuntime $Label_IceteaRuntime $Computed
union $Computed $Computed $Label_IceteaRuntime 

# OS
substract $Label_OS $Label_OS $Computed
union $Computed $Computed $Label_OS 
 
# VMALLOCATOR
substract $Label_VMAllocator $Label_VMAllocator $Computed
union $Computed $Computed $Label_VMAllocator

# VMCORE
substract $Label_VMCore $Label_VMCore $Computed
union $Computed $Computed $Label_VMCore
 
# Application Resources
substract $Label_ApplicationResources $Label_ApplicationResources $Computed
union $Computed $Computed $Label_ApplicationResources

# Application Immutables
substract $Label_ApplicationImmutables $Label_ApplicationImmutables $Computed
union $Computed $Computed $Label_ApplicationImmutables
 
# JavaApplication (without libraries)
substract $Label_JavaApplication $TotalSoar $Computed
union $Computed $Computed $Label_JavaApplication
 
# Remaining Data
substract $Remaining All $Computed
union $Computed $Computed $Remaining

