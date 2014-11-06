/**
 * 
 */
package proyecto_compiladores;

import java.awt.Desktop;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.UnsupportedEncodingException;
import java.io.Writer;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.FileDialog;
import org.eclipse.swt.widgets.Listener;
import org.eclipse.swt.widgets.Event;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.SWT;
import org.eclipse.swt.widgets.ToolBar;
import org.eclipse.swt.widgets.ToolItem;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.widgets.Text;
import org.eclipse.wb.swt.SWTResourceManager;
/**
 * @author colo
 *
 */
public class MainWindow {
	private static Text txtBlah;
	private static Shell shell;
	private static Shell anotherShell;
	private static Display display;
	private static String currentFile;

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		display = new Display();
		shell = new Shell(display);
		shell.setText("Self editor");
		
		ToolBar toolBar = new ToolBar(shell, SWT.FLAT | SWT.RIGHT);
		toolBar.setBounds(0, 0, 444, 23);
		
		ToolItem tltmTest = new ToolItem(toolBar, SWT.NONE);
		tltmTest.setText("Abrir");
		
		ToolItem tltmGuardar = new ToolItem(toolBar, SWT.NONE);
		tltmGuardar.setText("Guardar");
		
		ToolItem tltmAnalizar = new ToolItem(toolBar, SWT.NONE);
		tltmAnalizar.setText("Scanner");
		
		ToolItem tltmParser = new ToolItem(toolBar, SWT.NONE);
		tltmParser.setText("Parser");
		
		ToolItem tltmTest_1 = new ToolItem(toolBar, SWT.NONE);
		tltmTest_1.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(SelectionEvent e) {
			}
		});
		tltmTest_1.setText("Ayuda");
		
		
		// aquí pongo los manejadores de los click
		
	    class Open implements Listener {
	    	public void handleEvent(Event event) {
	    		FileDialog fd = new FileDialog(shell, SWT.OPEN);
	    		fd.setText("Open");
	    		fd.setFilterPath("~/");
	    		String[] filterExt = { "*.txt", "*.*" };
	    		fd.setFilterExtensions(filterExt);
	    		String selected = fd.open();
	    		currentFile = selected;
	    		if (selected != null) {
	    			displayFile(selected);
	    		}
	        }
	    }

	    class Save implements Listener {
	    	public void handleEvent(Event event) {
	    		FileDialog fd = new FileDialog(shell, SWT.SAVE);
	    		fd.setText("Open");
	    		fd.setFilterPath("~/");
	    		String[] filterExt = { "*.txt", "*.*" };
	    		fd.setFilterExtensions(filterExt);
	    		String selected = fd.open();
	    		System.out.println(selected);
	    		if (selected != null) {
	    			Writer writer;
	    			try {
	    				writer = new BufferedWriter(new OutputStreamWriter(
	    						new FileOutputStream(selected), "utf-8"));
	    				writer.write(txtBlah.getText());
	    				writer.close();
	    			} catch (UnsupportedEncodingException e) {
	    				e.printStackTrace();
	    			} catch (FileNotFoundException e) {
	    				System.out.println("file not found!");
	    				e.printStackTrace();
	    			} catch (IOException e) {
	    				System.out.println("IO exception!");
	    				e.printStackTrace();
	    			}
	    		}

	        }
	    }
	    
	    class Help implements Listener {
	    	public void handleEvent(Event event) {
	    		// TODO: escoger este PDF de manera dinamica?
	    		// cambiar la direccion a una relativa
	    		openPDF("/home/colo/Documents/clases/compi/proyecto_compiladores/ayuda.pdf");
	    	}
	    }
	    
		tltmTest.addListener(SWT.Selection, new Open());
		tltmGuardar.addListener(SWT.Selection, new Save());
		tltmTest_1.addListener(SWT.Selection, new Help());
		tltmAnalizar.addListener(SWT.Selection, new Listener() {
			public void handleEvent(Event e) {
				// TODO: escoger este archivo de manera dinamica?
				// TODO: escoger el proyecto de manera dinamica?
				// TODO: si no hay archivo, avisarlo!
				if (currentFile != null) {
					String line = runCommandAndReadOutput("/home/colo/Documents/clases/compi/proyecto_compiladores/proyecto " + currentFile);
					anotherShell = new Shell(display);
					anotherShell.setText("Analizer Output");				
					Text text = new Text(anotherShell, SWT.V_SCROLL);
					text.setBounds(5, 5, anotherShell.getBounds().width - 15, anotherShell.getBounds().height - 35);
					text.setText(line);
					text.setEditable(false);
					anotherShell.open();
					while (!anotherShell.isDisposed()) {
						if (!display.readAndDispatch()) display.sleep();
					}
				}
			}
		});
		tltmParser.addListener(SWT.Selection, new Listener() {
			public void handleEvent(Event e) {
				// TODO: escoger este archivo de manera dinamica?
				// TODO: escoger el proyecto de manera dinamica?
				// TODO: si no hay archivo, avisarlo!
				if (currentFile != null) {
					String line = runCommandAndReadOutput("/home/colo/Documents/clases/compi/proyecto_compiladores/parser2/fuentes/pl0 " + currentFile);
					String outputText = line;
					String[] splitLines = line.split("\n");
					int startText = 0;
					int endText = 0;
					int past = 0;
					boolean colorearAmarillo = false;
					Pattern errorPattern = Pattern.compile(".*Error.*");
					for(int i=0; i < splitLines.length; i++) {
						String str = splitLines[i];
						endText = startText + str.length();
						Matcher errorMatcher = errorPattern.matcher(str);
						if (errorMatcher.find()) {
							outputText = splitLines[past] + "\n" +  errorMatcher.group();
							colorearAmarillo = true;
							break;
						}
						past = i;
						startText += str.length();
					}
					anotherShell = new Shell(display);
					anotherShell.setText("Ventana de Resultados");
					Text text = new Text(anotherShell, SWT.V_SCROLL);
					if (colorearAmarillo) {
						text.setBackground(SWTResourceManager.getColor(SWT.COLOR_BLACK));
						text.setForeground(SWTResourceManager.getColor(SWT.COLOR_YELLOW));
					}
					text.setBounds(5, 5, anotherShell.getBounds().width - 15, anotherShell.getBounds().height - 35);
					text.setText(outputText);
					text.setEditable(false);
					anotherShell.open();
					while (!anotherShell.isDisposed()) {
						if (!display.readAndDispatch()) display.sleep();
					}
				}
			}
		});
		
		
		txtBlah = new Text(shell, SWT.V_SCROLL);
		txtBlah.setForeground(SWTResourceManager.getColor(SWT.COLOR_BLACK));
		txtBlah.setBackground(SWTResourceManager.getColor(SWT.COLOR_WIDGET_HIGHLIGHT_SHADOW));
		txtBlah.setBounds(5, 22, shell.getBounds().width - 15, shell.getBounds().height - 52);
		shell.addListener (SWT.Resize,  new Listener () {
		    public void handleEvent (Event e) {
		    	txtBlah.setBounds(5, 22, shell.getBounds().width - 15, shell.getBounds().height - 52);
		    }
		  });
		shell.open();
		while (!shell.isDisposed()) {
			if (!display.readAndDispatch()) display.sleep();
		}
		display.dispose();
	}
	
	public static void displayFile(String filepath) {
		String filestring = "";
		Path path = Paths.get(filepath);
		try {
			filestring = new String(Files.readAllBytes(path));
		} catch (IOException e) {
			filestring = "Error reading file " + filepath;
			e.printStackTrace();
		}
		txtBlah.setText(filestring);
	}
	
	public static String runCommandAndReadOutput(String cmd) {
		String lines = "";
	    try {
	    	String line;
			Process p = Runtime.getRuntime().exec(cmd);
			BufferedReader bri = new BufferedReader(new InputStreamReader(p.getInputStream()));
			while ((line = bri.readLine()) != null) {
//				System.out.println(line);
				lines = lines + "\n" + line;
		    }
			bri.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		return lines;
	}
	
	public static void runCommand(String cmd) {
	    try {
			Process p = Runtime.getRuntime().exec(cmd);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public static void openPDF(String filepath) {
		// TODO: probably should use the Desktop open thing
/*		File pdfFile = new File(filepath);
		if (Desktop.isDesktopSupported()) {
			try {
				Desktop.getDesktop().open(pdfFile);
			} catch (IOException e) {
				e.printStackTrace();
			}
		} else {
			System.out.println("Awt Desktop is not supported!");
		}
*/
		runCommand("okular " + filepath);
	}
}
