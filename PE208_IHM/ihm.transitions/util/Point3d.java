package util;

public class Point3d {

	public float x;
	public float y;
	public float z;

	private Point3d(float x, float y, float z) {
		super();
		this.x = x;
		this.y = y;
		this.z = z;
	}

	public static Point3d create(float x, float y, float z) {
		return new Point3d(x, y, z);
	}

}
