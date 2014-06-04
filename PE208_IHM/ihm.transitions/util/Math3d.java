package util;

public class Math3d {

	/**
	 * Calculates the 2d coordinates for the 3d coordinates. It returns the 2d
	 * coordinates as int[] array, where the x, y coordinates are successive.
	 * 
	 * @param coordinates
	 *            the array of point3d instances.
	 * @param an
	 *            int[number of points x 2] where the 2d coordinates will be
	 *            stored.
	 */
	private static void centralProjection(int centerX, int centerY,
			Point3d[] coordinates, int[] points, float POV) {

		for (int i = 0; i < coordinates.length; i++) {
			int index = i * 2;
			// x coordinates
			points[index] = centerX
					+ (int) ((coordinates[i].x * POV)
							/ (POV - coordinates[i].z) + 0.5);
			// y coordinates
			points[index + 1] = centerY
					+ (int) ((coordinates[i].y * POV)
							/ (POV - coordinates[i].z) + 0.5);
		}

	}

	/**
	 * Rotates along x axis.
	 * 
	 * @param source
	 *            the source array for the 3d coordinates.
	 * @param dest
	 *            the destination array, should have at least as many elements
	 *            as the source array. The values in the dest array will be
	 *            overwritten.
	 * @param xAngle
	 *            angle of rotation in radians
	 */
	private static void rotateX(Point3d[] source, Point3d[] dest, float xAngle) {
		for (int i = 0; i < source.length; i++) {
			float x = source[i].x;
			float y = source[i].y;
			float z = source[i].z;

			dest[i].x = x;
			dest[i].y = (float) (y * Math.cos(xAngle) + z * Math.sin(xAngle));
			dest[i].z = (float) (-y * Math.sin(xAngle) + z * Math.cos(xAngle));
		}
	}

	/**
	 * Rotates along x axis.
	 * 
	 * @param source
	 *            the source array for the 3d coordinates.
	 * @param dest
	 *            the destination array, should have at least as many elements
	 *            as the source array. The values in the dest array will be
	 *            overwritten.
	 * @param yAngle
	 *            angle of rotation in radians
	 */
	private static void rotateY(Point3d[] source, Point3d[] dest, float yAngle) {
		for (int i = 0; i < source.length; i++) {
			float x = source[i].x;
			float y = source[i].y;
			float z = source[i].z;
			dest[i].x = (float) (x * Math.cos(yAngle) - z * Math.sin(yAngle));
			dest[i].y = y;
			dest[i].z = (float) (x * Math.sin(yAngle) + z * Math.cos(yAngle));
		}
	}

	/**
	 * Rotate around Z axis
	 * 
	 * @param source
	 * @param dest
	 * @param zAngle
	 */
	private static void rotateZ(Point3d[] source, Point3d[] dest, float zAngle) {
		for (int i = 0; i < source.length; i++) {
			float x = source[i].x;
			float y = source[i].y;
			float z = source[i].z;

			dest[i].x = (float) (x * Math.cos(zAngle) - y * Math.sin(zAngle));
			dest[i].y = (float) (x * Math.sin(zAngle) + y * Math.cos(zAngle));
			dest[i].z = z;
		}
	}

	/**
	 * Rotates the source coordinates around the x,y,z axis and with central
	 * projection converts the 3d coordinates to 2d
	 * 
	 * @param source
	 *            source array of 3d coorindates
	 * @param xAngle
	 *            x rotation in rad
	 * @param yAngle
	 *            y rotation in rad
	 * @param zAngle
	 *            z rotation in rad
	 * @param result
	 *            int[] array the 2d coordinates will be stored here, the x,y
	 *            coordinates are successive
	 * @param pov
	 *            distance of the "eye" from the origo
	 */
	public static void get2dCoordinates(int centerX, int centerY,
			Point3d[] source, Point3d[] dest, int[] result, float xAngle,
			float yAngle, float zAngle, float pov) {

		copy(source, dest);

		if (xAngle != 0) {
			rotateX(dest, dest, xAngle);
		}
		if (yAngle != 0) {
			rotateY(dest, dest, yAngle);
		}
		if (zAngle != 0) {
			rotateZ(dest, dest, zAngle);
		}
		centralProjection(centerX, centerY, dest, result, pov);

	}

	/**
	 * Copies the data from source to dest
	 * 
	 * @param source
	 *            source array
	 * @param dest
	 *            destination array
	 */
	private static void copy(Point3d[] source, Point3d[] dest) {
		for (int i = 0; i < source.length; i++) {
			Point3d d = dest[i];
			Point3d s = source[i];
			d.x = s.x;
			d.y = s.y;
			d.z = s.z;

		}
	}

	public static Point3d[] initPoint3d(int numPoints) {
		Point3d[] point3ds = new Point3d[numPoints];
		for (int i = 0; i < point3ds.length; i++) {
			point3ds[i] = Point3d.create(0, 0, 0);
		}
		return point3ds;
	}

	/**
	 * Returns tile coordinates with the given size
	 * 
	 * @param tileWidth
	 *            width of the tile
	 * @param tileHeight
	 *            height of the tile
	 * @return the array of 3d coordinates
	 */
	public static Point3d[] getTileCoordinates(int tileWidth, int tileHeight) {
		return new Point3d[] {
				Point3d.create(-tileWidth / 2, -tileHeight / 2, 0),
				Point3d.create(tileWidth / 2, -tileHeight / 2, 0),
				Point3d.create(tileWidth / 2, tileHeight / 2, 0),
				Point3d.create(-tileWidth / 2, tileHeight / 2, 0) };
	}

}
