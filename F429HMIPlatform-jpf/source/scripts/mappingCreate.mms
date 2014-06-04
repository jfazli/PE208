# Copyright 2009-2013 IS2T. All rights reserved.

# This script only declares the graphs. The declared graph may contain more symbol than
# wanted (eg. VMCore includes VMAllocation symbols). Only union of graphs are allowed here.
# Unwanted symbols will be removed in computeGraphsXXX scripts.
#
# This script declares only common graphs. 
# Platform specific graphs should be added from specific createGraphsXXX script.
# Platform specific symbols should be added in common graph from specific createGraphsXXX script.
#
# When declaring a new graph, add all specific symbols (eg. com_is2t_display.*, ...) and also obfuscation prefix (eg. LIBDISPLAY.*)
#
# "Remaining" graph must be empty. All symbols and sections must be in graph. 

# ALL
createGraph All section=.*
createGraphNoRec $TotalSoar section=.soar.text.* section=.soar.rodata.* section=.soar.runtime.* section=.bss.soar.* _java_.*

# All statics
createGraphNoRec $Label_Statics section=COMMON

# RODATA_APPLI (resources+immutables)
createGraphNoRec $Label_ApplicationResources section=.soar.rodata.resource.* _java_rodata_resource.*
createGraphNoRec $Label_ApplicationImmutables  section=.soar.rodata.immutable.* _java_rodata_immutable.*

# Extract global .soar.runtime table  
createGraphNoRec $Label_JavaRuntime section=_ni_stackgroup_ESR_.* section=.bss.vm.stacks.gc section=.bss.vm.stacks.native section=.bss.vm.stacks.java _java_properties_Keys _java_properties_Values _java_sharedarray_start _java_stackgroup_table _java_names_data_start _java_soarfile_header_start

# Java strings
createGraphNoRec $Label_JavaStrings _java_sharedarray_start _java_internStrings_start

# JAVALIBS 
createGraphNoRec $JavaCLDCBON _java_Pjava_.* _java_B.* _java_ALjava_.* _java_AI.* _java_AC.* _java_Ljava_lang.* _java_Ljava_io.* _java_List_support.* _java_Ljava_util.* _java_[LP]ej_bon.* _java_[LP]ej_error.* _java_[LP]com_is2t_currenttime.* _java_[LP]javax_microedition.* _java_Pist_support.* _java_Pist _java_Pjava _java_Pej _java_Object_OutOfMemory _java_Lcom_is2t_tools.* _java_[LP]com_is2t_cldc.* _java_Lcom_is2t_vm_support_.* _java_Pcom_is2t_vm.* _java_Pcom_is2t_tools.*
createGraphNoRec $JavaPAP _java_[LP]ej_pap.* _java_[LP]com_is2t_pap.* _java_[LP]com_is2t_platform.*
createGraphNoRec $JavaECOM _java_[LP]ej_ecom.* _java_[LP]com_is2t_connector.* Java_ej_ecom_connection_.*
createGraphNoRec $JavaMicroUI _java_[LP]ej_microui.* _java_[LP]com_is2t_microbsp_microui.* _java_[LP]com_is2t_microui.* _java_[LP]com_is2t_debug_Assert.* _java_[LP]com_is2t_microej_microui.* _java_ALej_microui_.* _java_Pcom_is2t_microbsp.* _java_Lej_microui_.*
createGraphNoRec $JavaMWT _java_[LP]ej_mwt.* _java_[LP]com_is2t_mwt.* _java_ALej_mwt_.*
createGraphNoRec $JavaOSGiME _java_[LP]org_osgi_framework_.*
createGraphNoRec $JavaNLS _java_Lcom_is2t_nls_.* _java_Pcom_is2t_nls.*
createGraphNoRec $JavaSP _java_Lcom_is2t_sp_.* _java_Pcom_is2t_sp.*
createGraphNoRec $JavaStoryBoard _java_ALcom_is2t_gestures_.* _java_ALcom_is2t_storyboard_.* _java_Lcom_is2t_gestures_.* _java_Lcom_is2t_storyboard_.* _java_Pcom_is2t_storyboard.* _java_Pcom_is2t_gestures
createGraphNoRec $JavaWidgets _java_[LP]ej_mwt.widgets.* _java_[LP]com_is2t_mwt.widgets.*
createGraphNoRec $JavaInternetPack _java_[LP]com_is2t_server.* _java_[LP]com_is2t_template_parser.*

# JAVALIBS Native
createGraphNoRec $NativeCLDCBON section=IMMUTABLES_BLOCKS NATCLDCBON.* section=.text.NATCLDCBON.* section=.rodata.NATCLDCBON.* com_is2t_bon_.* ist_microjvm_NativesPool.* ist_microjvm_soar_NativesPoolLinker.* ist_microjvm_symbols.* ist_mowana_vm_GenericNativesPool.* ist_mowana_vm_GenericNativesPoolLinker.* ist_mowana_vm_immutablesBlock.* ist_mowana_vm_ImmutablesBlockNativesPool.* section=.rodata.ist_microjvm_symbols_ClassNameTable.*
createGraphNoRec $NativePAP NATPAP.* com_is2t_pap.* section=_pap_gpio_.* 
createGraphNoRec $NativeECOM _java_Lej_ecom.* com_is2t_ecom.* section=_ni_stackgroup_ECOM.* com_is2t_comm_.*
createGraphNoRec $NativeMicroUI section=_audio_out_stacks section=_input_pump_stacks section=_ni_stackgroup_MUI_.* NATMICROUI.* section=LCD.* section=lcd.* com_is2t_microui.* section=_andisplay_.* section=_display_.* section=.rodata.com_is2t_microui_.* section=.text.com_is2t_microui_.* section=.rodata.cosTable.* section=_touchscreen_table
createGraphNoRec $NativeMWT NATMWT.*
createGraphNoRec $NativeOSGiME NATOSGiME.*
createGraphNoRec $NativeNLS NATNLS.*
createGraphNoRec $NativeSP NATSP.* _java_Lcom_is2t_natives_sp_NShieldedPlug.* _java_Pcom_is2t_natives_sp section=.text.__icetea__getSingleton__com_is2t_sp_.* section=.text.__icetea__virtual__com_is2t_sp_.* section=.shieldedplug shieldedplug_.*
createGraphNoRec $NativeStoryBoard NATStoryBoard.*
createGraphNoRec $NativeWidgets NATWidgets.*


# JAVALIBS Java + Native + Immutables
union $Label_CLDC_B-ON $JavaCLDCBON $NativeCLDCBON
union $Label_PAP $JavaPAP $NativePAP
union $Label_ECOM $JavaECOM $NativeECOM
union $Label_MicroUI $JavaMicroUI $NativeMicroUI
union $Label_MWT $JavaMWT $NativeMWT
union $Label_OSGiME $JavaOSGiME $NativeOSGiME
union $Label_NLS $JavaNLS $NativeNLS
union $Label_SP $JavaSP $NativeSP
union $Label_StoryBoard $JavaStoryBoard $NativeStoryBoard
union $Label_Widgets $JavaWidgets $NativeWidgets
union $Label_Internet_Pack $JavaInternetPack $JavaInternetPack

# LIBNetwork
createGraphNoRec $Label_LIBNetwork LIBNETWORK.* lwip.* Macb.* .*NETWORK.*  .*mac.* .*socket.* com_ist_drivers_network.* Lwip.* com_is2t_lwip.* com_is2t_drivers_nationalsemiconductor_phy.* inputUnit=.*/lwip/.*lwip_.*\.o inputUnit=.*/libLwip\.a.* section=_macb_rx.* section=_macb_tx.* section=_LWIP_ReceptionSection  section=_ITThreadStack section=_TCPIP_ThreadStack

#DRIVER
createGraphNoRec $Label_Drivers DRIVER.* section=.rodata.com_is2t_drivers.* section=.text.DRIVER.* section=.rodata.DRIVER.* section=.text.__icetea__inline__DRIVER.* com_ist_drivers.* com_is2t_drivers.* time_CurrentTime.* icetea_os_internal_InternCounter.* section=.rodata.com_ist_drivers_.* section=.text.__icetea__inline__com_ist_drivers_.* section=.text.com_ist_drivers_.* freq__iceTea__initialData com_is2t_leds.*

#Icetea Runtime
createGraphNoRec $Label_IceteaRuntime section=.*HEAP.* section=EXTERNAL_ALLOCATOR _switch_.* ICETEART.* iceTea_lang.* com_ist_allocator.* com_is2t_startup.* ist_Default32bitsStartup.* ist_RegistryInitializationMethod.* com_ist_tools.* iceTea_boards.* __iceTea__.* .*_multiDimArrayAllocation_.* section=.text.com_ist_allocator_.* section=.text.iceTea_lang_.* section=.data.allocaPtr lib_externalAllocator__.* lib_math__iceTea__libInit section=.zero_init_table section=.relocation_data_table section=.rodata.icetea_components_init_table ist_SRelocationDataInterval.* ist_SZeroInitInterval.* inputUnit=.*/libgcc\.a.* section=.zero_init_table section=.rodata.zero_init_table section=.relocation_data_table section=.reset lib_externalAllocator.* lib_math.* _board_startup _program_start section=.text.vectors section=.rodata.relocation_data_table section=.text._board_initialization _icetea_support.* com_is2t_icetea_macro.* .*iceTea__initialData section=i.__iceTea__mallocINTERN section=.text.__icetea__getSingleton__iceTea_lang_.* section=.text.__icetea__virtual__iceTea_lang_.* 

#OS
createGraphNoRec $Label_OS OS.* section=.text.OS.* section=.rodata.OS.* icetea_os.* section=.text.icetea_os_.* section=.rodata.icetea_os_.* com_is2t_iceos_.*

# VMCORE
createGraphNoRec $Label_VMCore VMCORE.* section=.text.asm.VMCORE.* SNI_.* __icetea__inline__VMCORE.* section=.text.VMCORE.* section=.rodata.VMCORE.* ist_microjvm.* com_ist_vm_constants.* ist_mowana.* com_is2t_microjvm.* com_is2t_vm_mowana.* section=.rodata.vm.tableswitch section=.data.vm.tableswitch.reloc section=.rodata.ist_mowana_.* section=.text.ist_microjvm_.* section=.text.ist_mowana_.* com_is2t_mowana.* section=.text.__icetea__inline__VMCORE.* section=.bss.vm.tableswitch.reloc section=.text.asm.ON_.* section=.rodataVmTableswitch section=.text.__icetea__getSingleton__com_is2t_microjvm_.* section=.text.__icetea__virtual__com_is2t_microjvm_.* section=.text.__icetea__getSingleton__VMCOREMicroJvm.*

# VMALLOC
createGraphNoRec $Label_VMAllocator section=_java_heap.*  section=_java_immortals VMALLOC.* section=.text.VMALLOC.* section=.rodata.VMALLOC.* ist_microjvm_allocator.* ist_mowana_vm_allocator.* com_is2t_microjvm_mowana_allocator.* section=.rodata.ist_mowana_vm_allocator.*  section=.text.ist_mowana_vm_allocator.*

# VMCONSOLE
createGraphNoRec $Label_VMConsole VMCONSOLE.* ist_microConsole.* com_is2t_microjvm_console.* section=_CSL_.*

# VMDBG
createGraphNoRec $Label_VMDebugger VMDBG.* ist_debug.*

# STRINGS
createGraphNoRec $Label_Strings .*_ILS_.*

# LIBDISPLAY
createGraphNoRec $Label_LIBDisplay LIBDISPLAY.* __icetea__inline__LIBDISPLAY.* com_ist_display.* com_is2t_display.* section=.rodata.tabcosin.* section=.rodata.LIBDISPLAY.* com_ist_lcdDisplay.* section=.text.__icetea__inline__LIBDISPLAY.* section=.text.com_ist_display_.* section=.rodata.com_ist_display_.* section=.text.LIBDISPLAY.* section=.text.display.* _1display.* section=.text.__icetea__virtual__com_is2t_display.* section=HEAP_DISPLAY_STACK

# LIBINPUT
createGraphNoRec $Label_LIBInput LIBINPUT.* __icetea__inline__LIBINPUT.* com.is2t.inputs.* section=.text.LIBINPUT.* com_is2t_inputs.* MISCinputs.* _1input.* section=HEAP_INPUTS_STACK

# LIBPNG
createGraphNoRec $Label_LIBPNG LIBPNG.* section=.text.LIBPNG.* section=.rodata.LIBPNG.* section=.text.com_ist_display_image_png_.* section=.rodata.ADAM7.* com_is2t_zip.* section=.rodata.tableCRC.* section=.rodata.order.* section=.rodata.lbase.* section=.rodata.lext.* section=.rodata.dbase.* section=.rodata.dext.* section=.rodata.lenFixed.* section=.rodata.distFixed.* ADAM7.* com_is2t_display_image_PNGDecoder.* com_is2t_display_image_InternalImageDecoders___.*PNG 

# LIBBMPM
createGraphNoRec $Label_LIBBMPM com_is2t_display_image_BMPMDecoder.* com_is2t_display_image_InternalImageDecoders___.*BMPM

# LIBFLOAT
createGraphNoRec $Label_LIBFloat LIBFLOAT.* iceTea_lang_Double.* iceTea_lang_Float.* fmod fmodf 

# parse (Float/Double) toString(Float/Double)
createGraphNoRec $LIBFloatParseToString iceTea_lang_Double___parse.* iceTea_lang_Float___parse.* iceTea_lang_Double___toString.* iceTea_lang_Float___toString.* iceTea_lang_FPUtil.* section=.rodata.DOUBLE_F10__iceTea__initialData section=.rodata.DOUBLE_E10__iceTea__initialData section=.rodata.DOUBLE_F2__iceTea__initialData section=.rodata.DOUBLE_E2__iceTea__initialData section=.rodata.FLOAT_F10__iceTea__initialData section=.rodata.FLOAT_E10__iceTea__initialData section=.rodata.FLOAT_F2__iceTea__initialData section=.rodata.FLOAT_E2__iceTea__initialData  
union $Label_LIBFloat $Label_LIBFloat $LIBFloatParseToString

# Math (cos, sin, ...)
createGraphNoRec $LIBFloatMath iceTea_lang_Math.* __icetea__inline__iceTea_lang_Math.*
union $Label_LIBFloat $Label_LIBFloat $LIBFloatMath

# LIBFloat FPA
createGraphNoRec $LIBFloatFPA com_is2t_lib_FPA.* 
union $Label_LIBFloat $Label_LIBFloat $LIBFloatFPA

# LIBFloat GCC
createGraphNoRec $LIBFloatGCC LIBFloatGCC.*

createGraphNoRec $LIBFloatGCCArithmetic __addsf3 __adddf3 __addtf3 __addxf3 __subsf3 __subdf3 __subtf3 __subxf3 __mulsf3 __muldf3 __multf3 __mulxf3 __divsf3 __divdf3 __divtf3 __divxf3 __negsf2 __negdf2 __negtf2 __negxf2 
union $LIBFloatGCC $LIBFloatGCC $LIBFloatGCCArithmetic

createGraphNoRec $LIBFloatGCCConversion __extendsfdf2 __extendsftf2 __extendsfxf2 __extenddftf2 __extenddfxf2 __truncxfdf2 __trunctfdf2 __truncxfsf2 __trunctfsf2 __truncdfsf2 __fixsfsi __fixdfsi __fixtfsi __fixxfsi __fixsfdi __fixdfdi __fixtfdi __fixxfdi __fixsfti __fixdfti __fixtfti __fixxfti __fixunssfsi __fixunsdfsi __fixunstfsi __fixunsxfsi __fixunssfdi __fixunsdfdi __fixunstfdi __fixunsxfdi __fixunssfti __fixunsdfti __fixunstfti __fixunsxfti __floatsisf __floatsidf __floatsitf __floatsixf __floatdisf __floatdidf __floatditf __floatdixf __floattisf __floattidf __floattitf __floattixf __floatunsisf __floatunsidf __floatunsitf __floatunsixf __floatundisf __floatundidf __floatunditf __floatundixf __floatuntisf __floatuntidf __floatuntitf __floatuntixf
union $LIBFloatGCC $LIBFloatGCC $LIBFloatGCCConversion

createGraphNoRec $LIBFloatGCCComparaison __cmpsf2 __cmpdf2 __cmptf2 __unordsf2 __unorddf2 __unordtf2 __eqsf2 __eqdf2 __eqtf2 __nesf2 __nedf2 __netf2 __gesf2 __gedf2 __getf2 __ltsf2 __ltdf2 __lttf2 __lesf2 __ledf2 __letf2 __gtsf2 __gtdf2 __gttf2 
union $LIBFloatGCC $LIBFloatGCC $LIBFloatGCCComparaison

createGraphNoRec $LIBFloatGCCOther __powisf2 __powidf2 __powitf2 __powixf2 __mulsc3 __muldc3 __multc3 __mulxc3 __divsc3 __divdc3 __divtc3 __divxc3
union $LIBFloatGCC $LIBFloatGCC $LIBFloatGCCOther

# LIBFloat
union $Label_LIBFloat $Label_LIBFloat $LIBFloatGCC


# LIBInt GCC
createGraphNoRec $LIBIntGCC LIBIntGCC.*

createGraphNoRec $LIBIntGCCArithmetic __ashlsi3 __ashldi3 __ashlti3 __ashrsi3 __ashrdi3 __ashrti3 __divsi3 __divdi3 __divti3 __lshrsi3 __lshrdi3 __lshrti3 __modsi3 __moddi3 __modti3 __mulsi3 __muldi3 __multi3 __negdi2 __negti2 __udivsi3 __udivdi3 __udivti3 __udivmoddi3 __udivti3 __umodsi3 __umoddi3 __umodti3
union $LIBIntGCC $LIBIntGCC $LIBIntGCCArithmetic

createGraphNoRec $LIBIntGCCComparaison __cmpdi2 __cmpti2 __ucmpdi2 __ucmpti2
union $LIBIntGCC $LIBIntGCC $LIBIntGCCComparaison

createGraphNoRec $LIBIntGCCTrappingArithmetic __absvsi2 __absvdi2 __addvsi3 __addvdi3 __mulvsi3 __mulvdi3 __negvsi2 __negvdi2 __subvsi3 __subvdi3
union $LIBIntGCC $LIBIntGCC $LIBIntGCCTrappingArithmetic  

createGraphNoRec $LIBIntGCCBitOperations __clzsi2 __clzdi2 __clzti2 __ctzsi2 __ctzdi2 __ctzti2 __ffsdi2 __ffsti2 __paritysi2 __paritydi2 __parityti2 __popcountsi2 __popcountdi2 __popcountti2 __bswapsi2 __bswapdi2
union $LIBIntGCC $LIBIntGCC $LIBIntGCCBitOperations

# IceteaRuntime
union $Label_IceteaRuntime $Label_IceteaRuntime $LIBIntGCC

