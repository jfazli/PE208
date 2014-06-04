<?xml version="1.0"?>
<frontpanel 
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xmlns="http://xml.is2t.com/ns/1.0/frontpanel" 
	xsi:schemaLocation="http://xml.is2t.com/ns/1.0/frontpanel .fp1.0.xsd">
	
	<description file="widgets.desc"></description>
	
	<device name="SEB-F429" width="500" height="320">
		<body>
			<pixelatedDisplay id="0" x="10" y="10" width="480" height="272" initialColor="0x000000" extensionClass="com.is2t.microej.fp.SpecialDisplayExtension"/>
			<pointer id="0" x="10" y="10" width="480" height="272" touch="true" listenerClass="com.is2t.microej.fp.SpecialPointerListener"/>
	
			<led2states id="0" x="230" y="300" ledOff="led0_0.png" ledOn="led0_1.png" overlay="false"/>
			<led2states id="1" x="250" y="300" ledOff="led1_0.png" ledOn="led1_1.png" overlay="false"/>
		</body>
	</device>
</frontpanel>