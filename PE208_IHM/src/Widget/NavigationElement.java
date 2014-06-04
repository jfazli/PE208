package Widget;

import ej.mwt.Widget;

/**
 * This class describes the content of a element in the navigation. This element
 * provides a content for the current state, the title of the current state and
 * the state to go to when leaving this view.
 */
public interface NavigationElement {

	/**
	 * Returns the widget to display for the current state.
	 * 
	 * @return the widget to display for the current state
	 */
	public Widget getContent();

	/**
	 * Returns the title of the current state.
	 * 
	 * @return the current title
	 */
	public String getTitle();

}
