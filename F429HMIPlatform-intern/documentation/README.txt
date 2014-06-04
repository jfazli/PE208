# Copyright 2012-2013 IS2T. All rights reserved.
# For demonstration purpose only
# IS2T PROPRIETARY. Use is subject to license terms.

I) Introduction
===============
	This document describes the different parts of a MicroEJ XPF 
	(eXtensible PlatForm) example and step by step procedure for creating
	STM3240GEVAL-HMI-ARMCCv4 JPF and launching a Java application on it.
	
II) Content Description
=======================
	MicroEJ XPF example is split into multiple Eclipse projects:
	- [example]-intern:
		Contains ANT scripts for building the JPF (Java PlatForm)
	- [example]-bsp:
		Contains a ready-to-use package on STM3240GEVAL board, 
		including:
		- Keil MDK-ARM 4.5 project (compatible Keil MDK-ARM 4.6),
		- implementation of MicroJvm port on RTX OS,
		- STM3240GEVAL board support package.
	- [example]-mock (optional)
		A Java project that contains the code for a mock implementation of application Java
		native method(s). 
	- [example]-jpf
		This project will appear in workspace once Java platform is built. It 
		contains a JPF built for STM3240GEVAL board, ready to be 
		linked into Keil MDK-ARM project.
	- [example]-microuiGeneratorExtension (optional)
		Contains some files to extend MicroUI library
		
III) Launching Example
======================
	1) JPF build
		First step is to build a JPF for STM3240GEVAL board from CM4 
		compatible XPF. 
		Build may be less or more complex, depending on the set	of libraries 
		that will be provided by the JPF. Typical build steps are:
			- JPF instantiation,
			- fragments selection (native stacks, etc.),
			- Keil MDK-ARM project generation. 
		See content of [example]-intern/build.xml for more details.
		
		Procedure:
		- In Package Explorer, right-click on [example]-intern/build.xml 
		  > Run As > Ant Build.
		- Once ANT script is executed, [example]-jpf project shall appear in 
		  workspace.
		
	2) Java Application build 
		Second step is to write and generate a Java application on top of the 
		rightly built JPF.
		This platform has built-in samples.
		
		Procedure:
		- Click on File > New > Java Example
		- Select STM3240GEVAL-HMI-ARMCCv4 JPF
		- Select one of the available examples
		- Click on Next > Finish
		- In Package Explorer, right-click on Java example project > Run As 
		  > MicroEJ Application.
		- Once MicroEJ launch is executed, the object file shall have been 
		  generated in [example]-jpf/source/lib/javaapp.o.
		
	3) Final Application build
		Last step is to compile STM3240GEVAL board support package, link with 
		Java application, and program final application on board.
				
		Procedure:
		- In Package Explorer, right-click on [example]-bsp/Project/MicroEJ/
		  MDK-ARM/Project.uvproj > Open With > System Editor.
		- In Keil MDK-ARM Workbench, press F7. The C application is compiled and
		  linked with MicroJvm libraries and Java application.
		- Press Debug > Start/Stop Debug Session. (By default, project is 
		  configured for programming with STLINK/V2 probe).	  
		- Once application is loaded, press F5 for resuming. Java System.out 
		  print stream is redirected to C printf, which is connected to CN16 
		  USART connector (baudrate=115200,stopbits=1,parity=none,databits=8)
		

