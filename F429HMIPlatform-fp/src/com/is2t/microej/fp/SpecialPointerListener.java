/**
 * Copyright 2012 IS2T. All rights reserved
 * For demonstration purpose only.
 * IS2T PROPRIETARY. Use is subject to license terms.
 */
package com.is2t.microej.fp;

import com.is2t.microej.frontpanel.input.generator.EventGenerator;
import com.is2t.microej.frontpanel.input.listener.PointerListener;
import com.is2t.microej.microui.Constants;

public class SpecialPointerListener implements PointerListener {
	@Override
	public void move(int x, int y) {
		EventGenerator.sendPointerMovedEvent(Constants.TOUCH, x, y, true);
	}

	@Override
	public void press(int x, int y, int button) {
		EventGenerator.sendPointerPressedEvent(Constants.TOUCH, button, x, y, true);
	}

	@Override
	public void release(int x, int y, int button) {
		EventGenerator.sendPointerReleasedEvent(Constants.TOUCH, button);
	}
}
