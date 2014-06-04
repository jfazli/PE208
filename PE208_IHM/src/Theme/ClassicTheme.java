package Theme;

//import com.is2t.examples.mwt.helloworld.LabelRenderer;

import Composite.Renderer.PlaqueCompositeRenderer;
import Widget.Renderer.ButtonAnimationRenderer;
import Widget.Renderer.LabelRenderer;
import Widget.Renderer.PictureRenderer;
import Widget.Renderer.PoeleAnimationRenderer;
import Widget.Renderer.PuissancePlaqueLabelRenderer;
import Widget.Renderer.TemperatureLabelRenderer;
import Widget.Renderer.WirelessAnimationRenderer;
import ej.mwt.rendering.Look;
import ej.mwt.rendering.Theme;

public class ClassicTheme extends Theme {

	public Look getDefaultLook() {
		// TODO Auto-generated method stub
		return null;
	}

	public String getName() {
		// TODO Auto-generated method stub
		return "Classic Theme";
	}

	public boolean isStandard() {
		// TODO Auto-generated method stub
		return false;
	}

	protected void populate() {
		// TODO Auto-generated method stub
		add(new LabelRenderer());
		add(new PictureRenderer());
		add(new PlaqueCompositeRenderer());
		add(new PoeleAnimationRenderer());
		add(new WirelessAnimationRenderer());
		add(new TemperatureLabelRenderer());
		add(new PuissancePlaqueLabelRenderer());
		add(new ButtonAnimationRenderer());
	}

}
