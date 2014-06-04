/**
 * Copyright 2012 IS2T. All rights reserved
 * For demonstration purpose only.
 * IS2T PROPRIETARY. Use is subject to license terms.
 */
package com.is2t.microej.fp;

import com.is2t.microej.frontpanel.display.GenericDisplay16XYExtension;

public class SpecialDisplayExtension extends GenericDisplay16XYExtension {
	/**
	 * Default RGB masks for a pixel representation like:
	 * RRRR RGGG GGGB BBBB
	 */
	public static final int RED_MASK	= 0xf800;
	public static final int GREEN_MASK	= 0x07e0;
	public static final int BLUE_MASK	= 0x001f;
	
	/**
	 * widget-API
	 * Default standard conversion, sub classes can overrides this method.
	 * rgb color: xxxx xxxx RRRR Rxxx GGGG GGxx BBBB Bxxx
	 * lcd color:                     RRRR RGGG GGGB BBBB
	 * we keep 5 msbits R & B and 6 mbmits for G
	 */
	@Override
	public int convertRGBColorToDisplayColor(int rgbColor) {
		//			RED								GREEN							BLUE
		//      	XXXXX000.00000000              	00000XXX.XXX00000               00000000.000XXXXX
		rgbColor = ((rgbColor >>> 8) & RED_MASK) | ((rgbColor >>> 5) & GREEN_MASK) | ((rgbColor >>> 3) & BLUE_MASK);
		return (char)rgbColor;	// force cast on 16 bits
	}

	/**
	 * widget-API
	 * Default standard conversion, sub classes can overrides this method.
	 * lcd color:                     RRRR RGGG GGGB BBBB
	 * rgb color: 0000 0000 RRRR R000 GGGG GG00 BBBB B000
	 * we keep 5 msbits R & B and 6 mbmits for G
	 */
	@Override
	public int convertDisplayColorToRGBColor(int color) {
		int availableColor = color;
		//			RED								GREEN								BLUE
		//			XXXXX000.00000000              	00000XXX.XXX00000                  00000000.000XXXXX
		return ((availableColor & RED_MASK) << 8) | ((availableColor & GREEN_MASK) << 5) | ((availableColor & BLUE_MASK) << 3);
	}

	@Override
	public int getAlphaLevel() {
		return 0xFF;
	}

	@Override
	public int numberOfColors() {
		return (1 << 16) - 1;
	}

	@Override
	public boolean isColor() {
		return true;
	}

	@Override
	public boolean isDoubleBuffered() {
		return true;
	}
}
