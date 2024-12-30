import java.awt.*;
import java.awt.image.*;
import java.util.*;

/**
 * Region growing algorithm: finds and holds regions in an image.
 * Each region is a list of contiguous points with colors similar to a target color.
 * Scaffold for PS-1, Dartmouth CS 10, Fall 2016
 * 
 * @author Chris Bailey-Kellogg, Spring 2014 (based on a very different structure from Fall 2012)
 * @author Travis W. Peters, Dartmouth CS 10, Updated Spring 2015
 * @author CBK, Spring 2015, updated for CamPaint
 * @authors, Pauline Nyandusi and WinnieBrenda Waiya
 */
public class RegionFinder {
	private static final int maxColorDiff = 20;                // how similar a pixel color must be to the target color, to belong to a region
	private static final int minRegion = 50;                 // how many points in a region to be worth considering

	private BufferedImage image;                            // the image in which to find regions
	private BufferedImage recoloredImage;                   // the image with identified regions recolored

	private ArrayList<ArrayList<Point>> regions;            // a region is a list of points
	// so the identified regions are in a list of lists of points

	public RegionFinder() {
		this.image = null;
	}

	public RegionFinder(BufferedImage image) {
		this.image = image;
	}

	public void setImage(BufferedImage image) {
		this.image = image;
	}

	public BufferedImage getImage() {return image;
	}

	public BufferedImage getRecoloredImage() {
		return recoloredImage;
	}

	/**
	 * Sets regions to the flood-fill regions in the image, similar enough to the trackColor.
	 */
	public void findRegions(Color targetColor) {
		// TODO: YOUR CODE HERE
		regions = new ArrayList<ArrayList<Point>>();

		//keeps track of visited pixels
		BufferedImage visited = new BufferedImage(image.getWidth(), image.getHeight(), BufferedImage.TYPE_INT_ARGB);

		for (int y = 0; y <= image.getHeight()-1; y++) {
			for (int x = 0; x <= image.getWidth()-1; x++) {
                //if pixel is unvisited and the color matches
				if ((visited.getRGB(x, y) == 0) && colorMatch(new Color(image.getRGB(x, y)), targetColor)) {

					visited.setRGB(x, y, 1); //Marks as visited
					ArrayList<Point> newRegion = new ArrayList<>(); //start a new region
					ArrayList<Point> toVisit = new ArrayList<>(); //queue to keep track of pixels toVsisit

					toVisit.add(new Point(x, y));

					//System.out.println("add a point");

					while (toVisit.size() > 0) {
						Point a = toVisit.remove(toVisit.size() - 1);
						newRegion.add(a);
						//System.out.println("add a point");
						for (int ny = Math.max(0, a.y - 1); ny <= Math.min(image.getHeight()-1, a.y + 1); ny++) {
							for (int nx = Math.max(0, a.x - 1); nx <= Math.min(image.getWidth()-1, a.x + 1); nx++) {
								if ((visited.getRGB(nx, ny) == 0) && colorMatch(new Color(image.getRGB(nx, ny)), targetColor)) {
									visited.setRGB(nx, ny, 1); //mark as visited
									toVisit.add(new Point(nx, ny));
//									System.out.println("Add a point");
								}
							}

						}
					}
					if (newRegion.size() > minRegion) {
						regions.add(newRegion);

					}

					//System.out.println("add a point");

				}
			}
		}
	}

	/**
	 * Tests whether the two colors are "similar enough" (your definition, subject to the maxColorDiff threshold, which you can vary).
	 */
	private static boolean colorMatch(Color c1, Color c2) {
		// TODO: YOUR CODE HERE
		return (Math.abs(c1.getRed() - c2.getRed()) <= maxColorDiff) && (Math.abs(c1.getGreen() - c2.getGreen()) <= maxColorDiff) && (Math.abs(c1.getBlue() - c2.getBlue()))<=maxColorDiff;
	}

	/**
	 * Returns the largest region detected (if any region has been detected)
	 */
	public ArrayList<Point> largestRegion() {
		// TODO: YOUR CODE HERE

		ArrayList<Point> largestRegion = regions.get(0); //initialize the largest region as null
		int largestSize = largestRegion.size();
		for (ArrayList<Point> region : regions) {
			if (region.size() > largestSize) {
				largestSize = region.size();
				largestRegion = region;
			}
		}
		return largestRegion;
	}


	/**
	 * Sets recoloredImage to be a copy of image,
	 * but with each region a uniform random color,
	 * so we can see where they are
	 */
	public void recolorImage() {
		// First copy the original
		recoloredImage = new BufferedImage(image.getColorModel(), image.copyData(null), image.getColorModel().isAlphaPremultiplied(), null);
		// Now recolor the regions in it
		// TODO: YOUR CODE HERE
		for (ArrayList<Point> region : regions){
			int red = (int)(Math.random()* 256);
			int blue = (int)(Math.random()* 256);
			int green = (int)(Math.random()* 256);

			Color color = new Color(red,blue, green);
			for (Point P : region) {
				recoloredImage.setRGB(P.x, P.y, color.getRGB());
			}
		}
	}
}
