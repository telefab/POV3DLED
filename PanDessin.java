import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.geom.AffineTransform;
import java.awt.image.AffineTransformOp;
import java.awt.image.BufferedImage;
import java.awt.image.ColorModel;
import java.awt.image.ConvolveOp;
import java.awt.image.Kernel;
import java.awt.image.Raster;
import java.awt.image.RescaleOp;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import javax.imageio.ImageIO;
import javax.swing.JPanel;


public class PanDessin extends JPanel  {

  BufferedImage monImage = null;

	public PanDessin() {
		super();

	}

	protected void paintComponent(Graphics g)
	{
		super.paintComponent(g);
		if(monImage != null)
			g.drawImage(monImage, 0, 0, null);
	}


	protected void reduireImage()
	{
		BufferedImage imageReduite = new BufferedImage((int)(monImage.getWidth()*0.5),(int)( monImage.getHeight()*0.5), monImage.getType());
		AffineTransform reduire = AffineTransform.getScaleInstance(0.5, 0.5);
		int interpolation = AffineTransformOp.TYPE_BICUBIC;
		AffineTransformOp retaillerImage = new AffineTransformOp(reduire, interpolation);
		retaillerImage.filter(monImage, imageReduite );
		monImage = imageReduite ;
		repaint();
	}


	protected void agrandirImage()
	{
		BufferedImage imageZoomer = new BufferedImage((int)(monImage.getWidth()*1.5),(int)( monImage.getHeight()*1.5), monImage.getType());
		AffineTransform agrandir = AffineTransform.getScaleInstance(1.5, 1.5);
		int interpolation = AffineTransformOp.TYPE_BICUBIC;
		AffineTransformOp retaillerImage = new AffineTransformOp(agrandir, interpolation);
		retaillerImage.filter(monImage, imageZoomer );
		monImage = imageZoomer ;
		repaint();
	}


	

	protected int arrondi (int x){
		int valeurLimite=110;
		int r = 0;
		if (x>0 && x<=valeurLimite){
			r=0;
		}
				
		else if (x>valeurLimite && x<=255){
			r=1;
		}
		return r;
	}
	
	
	
	
	protected int moyenneRouge(int x, int y,Raster tramePixel,ColorModel modeleCouleur, int hauteur, int largeur){
		int moyenne=0;
		for (int j=x; j<(x+(monImage.getWidth()/largeur));j++ ){
	    	for (int i=y; i<(y+(monImage.getHeight()/hauteur));i++){
	    		Object objCouleur   = tramePixel.getDataElements(j, i, null);
	    		moyenne=moyenne+ (modeleCouleur.getRed(objCouleur) );
	    		
	    	}
		}
		//System.out.println(arrondi(moyenne/(hauteur*largeur)));
		//System.out.println(moyenne);
		return arrondi(moyenne/((monImage.getHeight()/hauteur)*(monImage.getWidth()/largeur)));
		
	}
	
	protected int moyenneBleu(int x, int y,Raster tramePixel,ColorModel modeleCouleur, int hauteur, int largeur){
		int moyenne=0;
		for (int j=x; j<(x+(monImage.getWidth()/largeur));j++ ){
	    	for (int i=y; i<(y+(monImage.getHeight()/hauteur));i++){
	    		Object objCouleur   = tramePixel.getDataElements(j, i, null);
	    		
	    		moyenne=moyenne+ (modeleCouleur.getBlue(objCouleur) );
	    	}
		}
		return arrondi(moyenne/((monImage.getHeight()/hauteur)*(monImage.getWidth()/largeur)));
	}
	
	protected int moyenneVert(int x, int y,Raster tramePixel,ColorModel modeleCouleur, int hauteur, int largeur){
		int moyenne=0;
		for (int j=x; j<(x+(monImage.getWidth()/largeur));j++ ){
	    	for (int i=y; i<(y+(monImage.getHeight()/hauteur));i++){
	    		//System.out.println(i);
	    		Object objCouleur   = tramePixel.getDataElements(j, i, null);
	    		moyenne=moyenne+ (modeleCouleur.getGreen(objCouleur) );
	    		
	    	}
		}
		return arrondi(moyenne/((monImage.getHeight()/hauteur)*(monImage.getWidth()/largeur)));
		
	}
	
	
	protected Color correspondanceCouleur(int r,int g, int b){
		Color c=Color.white;
		if (r==1 && g==0 && b==0)  {return Color.red;}
		else if (r==0 && g==1 && b==0) {return Color.green;}
		else if (r==0 && g==0 && b==1) {return Color.blue;}
		else if (r==0 && g==1 && b==1) {return Color.cyan;}
		else if (r==1 && g==1 && b==0) {return Color.yellow;}
		else if (r==1 && g==0 && b==1) {return Color.magenta;}
		else if (r==0 && g==0 && b==0) {return Color.black;}
		else if (r==1 && g==1 && b==1) {return Color.white;}
		return c;
	}
	
	
	protected int correspondanceNombre(Color c){
		int n=0;
		if (c==Color.red)  {return 0;}
		else if (c==Color.blue) {return 1;}
		else if (c==Color.green) {return 2;}
		else if (c==Color.cyan) {return 3;}
		else if (c==Color.yellow) {return 4;}
		else if (c==Color.magenta) {return 5;}
		else if (c==Color.white) {return 6;}
		return n;
	}
	
	
	protected void dessinerTable(){
		int hauteur =40;
		int largeur =monImage.getWidth()/(monImage.getHeight()/hauteur);
		BufferedImage image = new BufferedImage(monImage.getWidth(), monImage.getHeight(), BufferedImage.TYPE_INT_RGB);
		//on cree une matrice de couleurs
		Color [][] tab = new Color [hauteur] [largeur];
		int [][] res = new int [hauteur] [largeur];
		String ch="";
		
		Raster tramePixel = monImage.getRaster(); 
		ColorModel modeleCouleur = monImage.getColorModel();
		for (int j=0; j<largeur;j++ ){
	    	for (int i=0; i<hauteur;i++){
	    		int x=(j*(monImage.getWidth()/largeur));
	    		int y=(i*(monImage.getHeight()/hauteur)) ;	
	    		tab[i][j]=correspondanceCouleur(moyenneRouge(x,y,tramePixel,modeleCouleur,hauteur,largeur),moyenneVert(x,y,tramePixel,modeleCouleur,hauteur,largeur),moyenneBleu(x,y,tramePixel,modeleCouleur,hauteur,largeur));
	    	}
		}
		
		
		// on récupère le contexte graphique de la BufferedImage   
		Graphics2D g = image.createGraphics(); 
	    
	    for (int j=0; j<largeur;j++ ){
	    	for (int i=0; i<hauteur;i++){
	    		Color c=tab[i][j];
	    		
	    		g.setColor(c);
	    		g.fillRect(j*(monImage.getWidth()/largeur), i*(monImage.getHeight()/hauteur), (monImage.getWidth()/largeur), (monImage.getHeight()/hauteur));
	    		
	    	}
	    }
	    monImage = image;
	    repaint();
	    for (int i=0; i<hauteur;i++){
	    	for (int j=0; j<largeur;j++ ){
	    		res[i][j]=correspondanceNombre(tab[i][j]); 
	    		ch=ch+correspondanceNombre(tab[i][j]);
	    		ch=ch+",";
	    	}
	    	ch=ch+";";
	    }
	    System.out.println(ch);
	    
	    
	    //ecriture dans fichier
	    File f = new File ("matrice_image.txt");
	    try
	    {
	        FileWriter fw = new FileWriter (f);
	        fw.write (ch);
	        fw.close();
	    }
	    catch (IOException exception)
	    {
	        System.out.println ("Erreur lors de la lecture : " + exception.getMessage());
	    }
	    
	    
	}
	
	



	protected void ajouterImage(File fichierImage)
	{   // desiiner une image à l'ecran	
		try {
			monImage = ImageIO.read(fichierImage);
		} catch (IOException e) {
			e.printStackTrace();
		}
		repaint(); 
	}

	protected BufferedImage getImagePanneau()
	{      // récupérer une image du panneau
		int width  = this.getWidth();
		int height = this.getHeight();
		BufferedImage image = new BufferedImage(width, height,  BufferedImage.TYPE_INT_RGB);
		Graphics2D g = image.createGraphics();

		this.paintAll(g);
		g.dispose();
		return image;
	}

	protected void enregistrerImage(File fichierImage)
	{
		String format ="JPG";
		BufferedImage image = getImagePanneau();
		try {
			ImageIO.write(image, format, fichierImage);
		} catch (IOException e) {
			e.printStackTrace();
		}

	}
}
