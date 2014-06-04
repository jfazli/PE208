package Theme;

import ej.microui.Colors;
import ej.microui.io.DisplayFont;
import ej.microui.io.Font;

public class Skin {

	public static final int SCREEN_BACKGROUND_COLOR = 0x00;

	public static final int TEXT_COLOR = 0x10;
	public static final int TITLE_TEXT_COLOR = 0x11;
	public static final int TITLE_BACKGROUND_COLOR = 0x12;

	public static final int PICTO_TEXT_COLOR = 0x20;
	public static final int PICTO_BACKGROUND_COLOR = 0x21;
	public static final int PICTO_BACKGROUND_FOCUSED_COLOR = 0x22;

	public static final int MAIN_MENU_BUTTON_COLOR = 0x30;
	public static final int MAIN_MENU_BAR_BUTTON_FOCUSED_COLOR = 0x31;
	public static final int MAIN_MENU_LEGACY_FOCUSED_COLOR = 0x32;
	public static final int MAIN_MENU_LAST_FOCUSED_COLOR = 0x33;
	public static final int MAIN_MENU_MYKEY_FOCUSED_COLOR = 0x34;

	public static final int SCROLLBAR_BORDER_COLOR = 0x40;
	public static final int SCROLLBAR_INNER_COLOR = 0x41;
	public static final int SCROLLBAR_ELEVATOR_COLOR = 0x42;
	public static final int SCROLLBAR_CONTROL_COLOR = 0x43;

	public static final int LIST_ITEM_TEXT_COLOR = 0x50;
	public static final int LIST_ITEM_BACKGROUND_COLOR = 0x51;
	public static final int LIST_ITEM_BACKGROUND_FOCUSED_COLOR = 0x52;

	public static final int CARROUSEL_BACKGROUND_COLOR = 0x60;
	public static final int CARROUSEL_ARROW_COLOR = 0x61;
	public static final int CARROUSEL_TITLE_COLOR = 0x62;

	public static final int COUNTDOWN_BACKGROUND_COLOR = 0x70;
	public static final int COUNTDOWN_TEXT_COLOR = 0x71;
	public static final int COUNTDOWN_SYMBOL_LINE_COLOR = 0x72;
	public static final int COUNTDOWN_SYMBOL_FILLED_ARC_COLOR = 0x73;

	public static final int COOK_STEPS_BACKGROUND_COLOR = 0x80;
	public static final int COOK_STEPS_INACTIVE_LABEL_COLOR = 0x81;
	public static final int COOK_STEPS_INACTIVE_PICTO_COLOR = 0x82;
	public static final int COOK_STEPS_ORANGE_ACTIVE_COLOR = 0x83;
	public static final int COOK_STEPS_GREEN_ACTIVE_COLOR = 0x84;
	public static final int COOK_STEPS_RED_ACTIVE_COLOR = 0x85;

	public static final int PROGRESS_BAR_BACKGROUND_COLOR = 0x90;
	public static final int PROGRESS_BAR_INACTIVE_COLOR = 0x91;
	public static final int PROGRESS_BAR_EMPTY_MARK_COLOR = 0x92;

	public static final int MESSAGE_BOX_BACKGROUND_COLOR = 0xa0;
	public static final int MESSAGE_BOX_TITLE_COLOR = 0xa1;
	public static final int MESSAGE_BOX_TEXT_COLOR = 0xa2;

	public static final int SETTINGS_SCREEN_FOCUSED_COLOR = 0xb0;

	public static final int PROGRESS_BAR_LABEL_COLOR = 0xc0;

	// colors definitions
	private static final int COLOR_BLACK = 0x020202;
	private static final int COLOR_BLUE = 0x00a6b4;
	private static final int COLOR_ORANGE = 0xf38f00;
	private static final int COLOR_GREEN = 0x91c945;
	private static final int COLOR_RED = 0xe83330;
	private static final int COLOR_DARKGRAY = 0x333333;
	private static final int COLOR_LIGHTGRAY = 0xc3c3c3;

	// font
	public static final int FONT_UPPERCASE_BOLD_15 = 0x00;
	public static final int FONT_12 = 1 + FONT_UPPERCASE_BOLD_15;
	public static final int FONT_10 = 1 + FONT_12;
	public static final int FONT_28 = 1 + FONT_10;
	public static final int FONT_NUMBERS_BOLD_40 = 1 + FONT_28;

	public static final int MAIN_MENU_BUTTON_FONT = FONT_10;
	public static final int BIG_TITLE_FONT = FONT_28;
	public static final int HUGE_TITLE_FONT = FONT_28;
	public static final int LIST_ITEM_FONT = FONT_10;
	public static final int TEXT_FONT = FONT_28;//FONT_12;
	public static final int PICTO_FONT = FONT_12;
	public static final int PICTO_ONGLET_FONT = FONT_UPPERCASE_BOLD_15;
	public static final int PICTO_CHANGE_VALUE_FONT = FONT_28;
	public static final int COOKSTEP_FONT = FONT_UPPERCASE_BOLD_15;
	public static final int COUNTDOWN_FONT = FONT_NUMBERS_BOLD_40;
	public static final int MESSAGE_BOX_TITLE_FONT = HUGE_TITLE_FONT;
	public static final int MESSAGE_BOX_FONT = FONT_UPPERCASE_BOLD_15;

	public static int getColor(int index) {
		switch (index) {
		case SCREEN_BACKGROUND_COLOR:
			return COLOR_BLACK;

		case TEXT_COLOR:
			return Colors.WHITE;
		case TITLE_TEXT_COLOR:
			return Colors.WHITE;
		case TITLE_BACKGROUND_COLOR:
			return COLOR_DARKGRAY;

		case PICTO_TEXT_COLOR:
			return Colors.WHITE;
		case PICTO_BACKGROUND_COLOR:
			return COLOR_DARKGRAY;
		case PICTO_BACKGROUND_FOCUSED_COLOR:
			return COLOR_RED;

		case MAIN_MENU_BUTTON_COLOR:
			return COLOR_DARKGRAY;
		case MAIN_MENU_BAR_BUTTON_FOCUSED_COLOR:
			return COLOR_LIGHTGRAY;

		case PROGRESS_BAR_LABEL_COLOR:
			return Colors.WHITE;

		case MAIN_MENU_LEGACY_FOCUSED_COLOR:
			return COLOR_RED;
		case MAIN_MENU_LAST_FOCUSED_COLOR:
			return COLOR_GREEN;
		case MAIN_MENU_MYKEY_FOCUSED_COLOR:
			return COLOR_ORANGE;

		case SCROLLBAR_BORDER_COLOR:
			return Colors.WHITE;
		case SCROLLBAR_INNER_COLOR:
			return COLOR_BLACK;
		case SCROLLBAR_ELEVATOR_COLOR:
			return COLOR_RED;
		case SCROLLBAR_CONTROL_COLOR:
			return COLOR_RED;

		case LIST_ITEM_TEXT_COLOR:
			return Colors.WHITE;
		case LIST_ITEM_BACKGROUND_COLOR:
			return COLOR_BLACK;
		case LIST_ITEM_BACKGROUND_FOCUSED_COLOR:
			return COLOR_RED;

		case CARROUSEL_BACKGROUND_COLOR:
			return COLOR_DARKGRAY;
		case CARROUSEL_ARROW_COLOR:
			return COLOR_RED;
		case CARROUSEL_TITLE_COLOR:
			return Colors.WHITE;

		case COUNTDOWN_BACKGROUND_COLOR:
			return COLOR_DARKGRAY;
		case COUNTDOWN_TEXT_COLOR:
			return Colors.WHITE;
		case COUNTDOWN_SYMBOL_LINE_COLOR:
			return Colors.WHITE;
		case COUNTDOWN_SYMBOL_FILLED_ARC_COLOR:
			return COLOR_RED;

		case COOK_STEPS_BACKGROUND_COLOR:
			return COLOR_DARKGRAY;

		case COOK_STEPS_INACTIVE_LABEL_COLOR:
			return COLOR_LIGHTGRAY;
		case COOK_STEPS_INACTIVE_PICTO_COLOR:
			return COLOR_LIGHTGRAY;

		case PROGRESS_BAR_BACKGROUND_COLOR:
			return COLOR_DARKGRAY;
		case PROGRESS_BAR_EMPTY_MARK_COLOR:
			return Colors.WHITE;
		case PROGRESS_BAR_INACTIVE_COLOR:
			return COLOR_DARKGRAY;

		case COOK_STEPS_GREEN_ACTIVE_COLOR:
			return COLOR_GREEN;
		case COOK_STEPS_ORANGE_ACTIVE_COLOR:
			return COLOR_ORANGE;
		case COOK_STEPS_RED_ACTIVE_COLOR:
			return COLOR_RED;

		case MESSAGE_BOX_BACKGROUND_COLOR:
			return COLOR_BLACK;
		case MESSAGE_BOX_TITLE_COLOR:
			return COLOR_RED;
		case MESSAGE_BOX_TEXT_COLOR:
			return Colors.WHITE;

		case SETTINGS_SCREEN_FOCUSED_COLOR:
			return Colors.BLUE;

		default:
			System.out.println("Unknown color index");
			return 0x000000;
		}
	}

	private static DisplayFont fontUppercaseBold15 = DisplayFont.getFont(63,
			15, Font.STYLE_BOLD);
	private static DisplayFont font12 = DisplayFont.getFont(65, 16,
			Font.STYLE_PLAIN);
	private static DisplayFont font28 = DisplayFont.getFont(64, 24,
			Font.STYLE_PLAIN);
	private static DisplayFont font10 = DisplayFont.getFont(67, 14,
			Font.STYLE_PLAIN);
	private static DisplayFont fontNumbersBold40 = DisplayFont.getFont(62, 40,
			Font.STYLE_BOLD);

	public static DisplayFont getFont(int index) {
		switch (index) {
		case FONT_NUMBERS_BOLD_40:
			return fontNumbersBold40;
		case FONT_12:
			return font12;
		case FONT_10:
			return font10;
		case FONT_28:
			return font28;
		case FONT_UPPERCASE_BOLD_15: // fall down
		default:
			return fontUppercaseBold15;
		}
	}

}
