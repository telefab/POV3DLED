import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.WindowConstants;

public class Cadre extends JFrame implements ActionListener {

  private final JMenuBar menuBar = new JMenuBar();
	private final JMenu fichierMenu = new JMenu();
	private final JMenuItem ouvrirMenu = new JMenuItem();
	private final PanDessin panneau = new PanDessin();
	private final JMenuItem enregistrerMenu = new JMenuItem();
	private final JMenu retaillerMenu = new JMenu();
	private final JMenuItem agrandirMenu = new JMenuItem();
	private final JMenuItem reduireMenu = new JMenuItem();
	private final JMenuItem dessinerTableMenu = new JMenuItem();

	public Cadre() {
		super();
		setBounds(100, 100, 500, 375);
		setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);
		try {
			creerMenu();
		} catch (Throwable e) {
			e.printStackTrace();
		}
		//
	}
	private void creerMenu() throws Exception {

		// construction du menu
		setJMenuBar(menuBar);	
		menuBar.add(fichierMenu);
		fichierMenu.setText("Fichier");
		fichierMenu.add(ouvrirMenu);
		ouvrirMenu.addActionListener((ActionListener)this);
		ouvrirMenu.setText("ouvrir");

		fichierMenu.add(enregistrerMenu);
		enregistrerMenu.addActionListener((ActionListener)this);
		enregistrerMenu.setText("enregistrer");		

		menuBar.add(retaillerMenu);
		retaillerMenu.setText("retailler");

		retaillerMenu.add(agrandirMenu);
		agrandirMenu.addActionListener((ActionListener)this);
		agrandirMenu.setText("agrandir");

		retaillerMenu.add(reduireMenu);
		reduireMenu.addActionListener((ActionListener)this);
		reduireMenu.setText("reduire");
		
		retaillerMenu.add(dessinerTableMenu);
		dessinerTableMenu.addActionListener((ActionListener)this);
		dessinerTableMenu.setText("dessinerTable");

		// ajouter le panneau de dessin
		getContentPane().add(panneau);
	}
	public void actionPerformed(ActionEvent cliqueMenu) {
		if (cliqueMenu.getSource().equals(ouvrirMenu))
		{
			JFileChooser fileOuvrirImage = new JFileChooser();
			if (fileOuvrirImage.showOpenDialog(this) == JFileChooser.APPROVE_OPTION) {
				panneau.ajouterImage(new File(fileOuvrirImage.getSelectedFile()
						.getAbsolutePath()));
			}
		} else if (cliqueMenu.getSource().equals(enregistrerMenu)) {
			JFileChooser fileEnregistrerImage = new JFileChooser();
			if (fileEnregistrerImage.showSaveDialog(this) == JFileChooser.APPROVE_OPTION) {
				File fichierEnregistrement = new File(fileEnregistrerImage.getSelectedFile().getAbsolutePath()+ ".JPG");
				panneau.enregistrerImage(fichierEnregistrement);
			}
		} else
			 if (cliqueMenu.getSource().equals(agrandirMenu)) {
				panneau.agrandirImage();
			} else if (cliqueMenu.getSource().equals(reduireMenu)) {
				panneau.reduireImage();
			}else if(cliqueMenu.getSource().equals(dessinerTableMenu)){
				panneau.dessinerTable();
			}
	}

	public static void main(String args[]) 
	{
		try {
			Cadre frame = new Cadre();
			frame.setVisible(true);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
