<!--
	FNI
	
	Copyright 2011 IS2T. All rights reserved.
	Modification and distribution is permitted under certain conditions.
	IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
-->
<nativesInterface>
	<nativesPool name="com.is2t.microui.MicroUIAlphaDisplayNativesPool">
		<native name="com.is2t.microbsp.microui.natives.NSystemAlphaNumericDisplay.init(int,int)boolean"/>
		
		<native name="com.is2t.microbsp.microui.natives.NSystemAlphaNumericDisplay.getWidth(int)int"/>
		<native name="com.is2t.microbsp.microui.natives.NSystemAlphaNumericDisplay.getHeight(int)int"/>
		<native name="com.is2t.microbsp.microui.natives.NSystemAlphaNumericDisplay.isColor(int)boolean"/>
		<native name="com.is2t.microbsp.microui.natives.NSystemAlphaNumericDisplay.getNumberOfColors(int)int"/>
		<native name="com.is2t.microbsp.microui.natives.NSystemAlphaNumericDisplay.hasBacklight(int)boolean"/>
		
		<native name="com.is2t.microbsp.microui.natives.NSystemAlphaNumericDisplay.setContrast(int,int)void"/>
		<native name="com.is2t.microbsp.microui.natives.NSystemAlphaNumericDisplay.getContrast(int)int"/>
		<native name="com.is2t.microbsp.microui.natives.NSystemAlphaNumericDisplay.backlightOn(int,boolean)void"/>
		<native name="com.is2t.microbsp.microui.natives.NSystemAlphaNumericDisplay.setBacklight(int,int)void"/>
		<native name="com.is2t.microbsp.microui.natives.NSystemAlphaNumericDisplay.getBacklight(int)int"/>
		
		<native name="com.is2t.microbsp.microui.natives.NSystemAlphaNumericDisplay.setCursorPosition(int,int,int)void"/>
		<native name="com.is2t.microbsp.microui.natives.NSystemAlphaNumericDisplay.getCursorPositionX(int)int"/>
		<native name="com.is2t.microbsp.microui.natives.NSystemAlphaNumericDisplay.getCursorPositionY(int)int"/>
		<native name="com.is2t.microbsp.microui.natives.NSystemAlphaNumericDisplay.showCursor(int)void"/>
		<native name="com.is2t.microbsp.microui.natives.NSystemAlphaNumericDisplay.hideCursor(int)void"/>
		<native name="com.is2t.microbsp.microui.natives.NSystemAlphaNumericDisplay.blinkCursor(int)void"/>
		
		<native name="com.is2t.microbsp.microui.natives.NSystemAlphaNumericDisplay.clear(int,boolean)void"/>
		<native name="com.is2t.microbsp.microui.natives.NSystemAlphaNumericDisplay.setColor(int,int)void"/>
		<native name="com.is2t.microbsp.microui.natives.NSystemAlphaNumericDisplay.getColor(int,int)int"/>
		<native name="com.is2t.microbsp.microui.natives.NSystemAlphaNumericDisplay.drawChars(int,int,int,char[],int,int,boolean)void"/>
		<native name="com.is2t.microbsp.microui.natives.NSystemAlphaNumericDisplay.flushDisplay(int)void"/>
		
		<native name="com.is2t.microbsp.microui.natives.NSystemAlphaNumericDisplay.setData(int,int,char[],int,int,boolean,boolean)boolean"/>
		<native name="com.is2t.microbsp.microui.natives.NSystemAlphaNumericDisplay.createScroll(int,int,int,int,int,int,int)int"/>
		<native name="com.is2t.microbsp.microui.natives.NSystemAlphaNumericDisplay.updateScroll(int,int,int)boolean"/>	
		<native name="com.is2t.microbsp.microui.natives.NSystemAlphaNumericDisplay.moveScrolls(int)void"/>
		<native name="com.is2t.microbsp.microui.natives.NSystemAlphaNumericDisplay.updateScrollWait(int,int,int)void"/>
		
	</nativesPool>
</nativesInterface>