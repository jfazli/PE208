/**
 * Java
 * Copyright 2012 IS2T. All rights reserved
 * For demonstration purpose only
 * IS2T PROPRIETARY / Use is subject to license terms.
 */
package com.is2t.microui.generators.extension;

import com.is2t.microej.microui.image.GenericDisplay16XYExtension;

public class MicroUIGeneratorExtensionForGEVAL implements GenericDisplay16XYExtension{

	@Override
	public int getID() {
		return 0;
	}
	
	public int convertRGBColorToDisplayColor(int rgbColor) {
		return (char) ((rgbColor & 0xf80000) >>> 8) | ((rgbColor & 0x00fc00) >>> 5) | ((rgbColor & 0xf8) >>> 3);	
	}
}
