import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.lang.Exception;
import java.util.HashMap;
import java.util.HashSet;
import java.util.StringTokenizer;

import javafx.animation.KeyFrame;
import javafx.animation.KeyValue;
import javafx.animation.Timeline;
import javafx.application.Application;
import javafx.application.Platform;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.scene.Scene;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.control.Menu;
import javafx.scene.control.MenuBar;
import javafx.scene.control.MenuItem;
import javafx.scene.control.SeparatorMenuItem;
import javafx.scene.image.Image;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;
import javafx.util.Duration;

public class MainWindow extends Application {

    /**
     * The path to the configuration file.
     */
    private static String config;
    /**
     * The client object of the display.
     * @see Client
     */
    private static Client client;
    /**
     * A Timeline object to refresh the display with the {@link MainWindow#FPS}
     */
    private Timeline timeline;
    /**
     * A console object, it created in the display
     * @see console
     */
    private Console console;
    /**
     * Frames per second, 60 by default
     */
    private int FPS = 60;

    @Override
    public void init() throws Exception {
        client = new Client(config);
    }

    @Override
    /**
     * Overrided from javafx, here where the main window, console and buttons are constructed, and where the display is refreshed.
     */
    public void start(Stage primaryStage) throws Exception{
        VBox vb = new VBox();
        MenuBar menuBar = new MenuBar();
        Menu menu = new Menu("Options");
        MenuItem openConsole = new MenuItem("Open Console");
        MenuItem logOut = new MenuItem("Log out");
        MenuItem logOutExit = new MenuItem("Log out and Exit");
        SeparatorMenuItem separator = new SeparatorMenuItem();
        Canvas drawingCanvas = new Canvas(720, 715);

        Image background = new Image(new FileInputStream(new File("../src/client/" + client.getResources() + "/background.jpg")));
                
        GraphicsContext gc = drawingCanvas.getGraphicsContext2D();
        HashMap<String, Image> map = new HashMap<String, Image>();

        String path = "../src/client/" + client.getResources() + "/";
        map.put("orange", new Image(new FileInputStream(new File(path + "orange.png"))));
        map.put("blue", new Image(new FileInputStream(new File(path + "blue.png"))));
        map.put("samaka", new Image(new FileInputStream(new File(path + "samaka.png"))));
        map.put("white", new Image(new FileInputStream(new File(path + "white.png"))));
        map.put("default", new Image(new FileInputStream(new File(path + "orange.png"))));
        
        timeline = new Timeline();
        timeline.setCycleCount(Timeline.INDEFINITE);
        timeline.getKeyFrames().add(
            new KeyFrame(Duration.millis(1000. / (double)FPS),
                new EventHandler<ActionEvent>() {

                    @Override
                    public void handle(ActionEvent arg0) {
                        gc.clearRect(0, 0, drawingCanvas.getWidth(), drawingCanvas.getHeight());
                        gc.drawImage(background , 0, 0, drawingCanvas.getWidth(), drawingCanvas.getHeight());
                        for (Fish f : new HashSet<Fish>(client.getFishs())) {
                            f.updatePos(FPS);
                            if (map.containsKey(fixName(f.getName()))) {
                                gc.drawImage(map.get(fixName(f.getName())),
                                             f.getXpos() * drawingCanvas.getWidth() / 100, 
                                             f.getYpos() * drawingCanvas.getHeight() / 100,
                                             f.getWidth() * drawingCanvas.getWidth() / 100,
                                             f.getHeight() * drawingCanvas.getHeight() / 100);
                            }
                            else {
                                gc.drawImage(map.get("default"),  
                                             f.getXpos() * drawingCanvas.getWidth() / 100, 
                                             f.getYpos() * drawingCanvas.getHeight() / 100,
                                             f.getWidth()* drawingCanvas.getWidth() / 100,
                                             f.getHeight() * drawingCanvas.getHeight() / 100);
                            }
                        }
                    }
                    
                },
                new KeyValue[0])
        );
        timeline.playFromStart();

        openConsole.setOnAction(new EventHandler<ActionEvent>(){
            @Override
            public void handle(ActionEvent event) {
                console = new Console(client);
                Scene consoleScene = new Scene(console.getRoot());
                console.setScene(consoleScene);
                console.show();
            }
        });

        logOutExit.setOnAction(new EventHandler<ActionEvent>(){
            @Override
            public void handle(ActionEvent event) {
                Platform.exit();
            }
        });

        logOut.setOnAction(new EventHandler<ActionEvent>(){
            @Override
            public void handle(ActionEvent arg0) {
                try {
                    client.communicate("log out");
                }
                catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });

        menuBar.getMenus().add(menu);
        menu.getItems().addAll(openConsole, separator, logOut, logOutExit);
        vb.getChildren().addAll(menuBar, drawingCanvas);

        Scene primaryScene = new Scene(vb, 720, 720);
        primaryStage.setTitle("Client");
        primaryStage.setResizable(false);
        primaryStage.setScene(primaryScene);
        primaryStage.show();
    }

    @Override
    /**
     * THis function is called after the window is closed.
     */
    public void stop() {
        try {
            client.communicate("disconnect");
            client.close();
        } catch(Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * Fixes the name of the fish, if {@code name == fishname_xx} the new name returned is {@code fishname}
     * @param name Name of the fish
     * @return The fixed name
     */        
    private String fixName(String name) {
        StringTokenizer st = new StringTokenizer(name, "_");
        return st.nextToken();
    }

    public static void main(String[] args) {
        config = args[0];
        launch();
    }

}