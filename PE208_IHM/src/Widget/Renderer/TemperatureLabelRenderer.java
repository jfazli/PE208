package Widget.Renderer;

import Theme.Skin;
import Widget.Label;
import Widget.TemperatureLabel;
import ej.microui.Colors;
import ej.microui.io.DisplayFont;
import ej.microui.io.GraphicsContext;
import ej.mwt.MWT;
import ej.mwt.Renderable;
import ej.mwt.Widget;
import ej.mwt.rendering.WidgetRenderer;

public class TemperatureLabelRenderer extends WidgetRenderer {
	private static int LINE_SEPARATOR = '\n';
	private static int PADDING = 5;

	public Class getManagedType() {
		return TemperatureLabel.class;
	}

	public int getPreferredContentHeight(Widget widget) {
		TemperatureLabel label = (TemperatureLabel) widget;
		String text = label.getText();

		if (text == null) {
			return 0;
		} else {
			return _getLineCount(text) * getFont().getHeight();
		}
	}

	public int getPadding() {
		return PADDING;
	}

	public int getPreferredContentWidth(Widget widget) {
		TemperatureLabel label = (TemperatureLabel) widget;
		String text = label.getText();

		if (text == null) {
			return 0;
		} else {
			return _getMaxWidth(text, getFont());
		}
	}

	public void render(GraphicsContext g, Renderable renderable) {
		TemperatureLabel label = (TemperatureLabel) renderable;
		int width = label.getWidth();
		int height = label.getHeight();
		String text = label.getText();
		int padding = getPadding();

		// label
		if (text != null /*&& label.getValue() != 0*/) {
			DisplayFont font = getFont();
			String[] lines = _getLines(text);
			int count = lines.length;
			int lineHeight = font.getHeight();
			int textHeight = count * lineHeight;
			int y = (height - textHeight) / 2 + textHeight / (2 * count);
			int align = GraphicsContext.VCENTER;

			// width is reused to contains x position
			switch (label.getAlignment()) {
			case MWT.RIGHT:
				width -= padding;
				align |= GraphicsContext.RIGHT;
				break;

			case MWT.CENTER:
				width = width >> 1;
				align |= GraphicsContext.HCENTER;
				break;

			default:
				width = padding;
				align |= GraphicsContext.LEFT;
				break;
			}

			g.setFont(font);
			g.setColor(getTextColor());

			for (int i = -1; ++i < count;) {
				g.drawString(lines[i], width, y, align);
				y += lineHeight;
			}
		}
	}

	protected int getTextColor() {
		return Skin.getColor(Skin.TITLE_TEXT_COLOR);
	}

	protected DisplayFont getFont() {
		return Skin.getFont(Skin.TEXT_FONT);
	}

	private int _getMaxWidth(String text, DisplayFont font) {
		String[] lines = _getLines(text);
		int count = lines.length;
		int width = 0;

		for (int i = count; --i >= 0;) {
			width = Math.max(width, font.stringWidth(lines[i]));
		}

		return width;
	}

	private int _getLineCount(String text) {
		int count = 1;
		char[] chars = text.toCharArray();
		int length = chars.length;

		for (int i = length; --i >= 0;) {
			if (chars[i] == LINE_SEPARATOR) {
				count++;
			}
		}

		return count;
	}

	private String[] _getLines(String text) {
		String[] lines = new String[_getLineCount(text)];
		int count = -1;
		int start = 0;
		int index = text.indexOf(LINE_SEPARATOR, start);

		while (index != -1) {
			lines[++count] = text.substring(start, index);
			start = index + 1;
			index = text.indexOf(LINE_SEPARATOR, start);
		}
		lines[++count] = text.substring(start);

		return lines;
	}

}
