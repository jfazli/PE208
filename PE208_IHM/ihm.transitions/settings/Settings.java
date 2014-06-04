package settings;

/**
 * Various settings for the application
 */
public class Settings {

	/**
	 * If true the tiles can be rotated both the x and y axis. This option
	 * causes very slow refresh speed for hardware without floating point
	 * support.
	 */
	public final static boolean ENABLE_TWO_AXIS_ROTATION = false;

	/**
	 * if enabled very fine detailed log messages are printed to the console.
	 * Could be enabled for debugging.
	 */
	public final static boolean LOG_TRACE = true;

	/**
	 * If enabled coarse grained log messages are printed to the console. Could
	 * be enabled for debugging.
	 */
	public final static boolean LOG_INFO = false;

	/**
	 * If enabled error log messages are printed to the console. Could be
	 * enabled for debugging.
	 */
	public final static boolean LOG_ERROR = true;

}
