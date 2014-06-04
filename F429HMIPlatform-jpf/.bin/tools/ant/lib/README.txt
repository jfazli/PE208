The Ant-Launcher library has been modified to prevent 
irrelevant error message during the use of the RCP software.
In accordance with the Apache license, IS2T hereby informs 
YOU that the getToolsJar() method has been modified that way :
The line "System.out.println("Unable to locate tools.jar."
+ "Expected to find it in " + toolsJar.getPath());” has been 
replaced by the opcode "nop".
