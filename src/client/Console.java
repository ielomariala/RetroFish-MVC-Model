import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import javafx.event.EventHandler;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.TextArea;
import javafx.scene.control.TextField;
import javafx.scene.input.KeyCode;
import javafx.scene.input.KeyEvent;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;

public class Console {
    /**
     * The client used in the console.
     * @see Client
     */
    private static Client client;
    /**
     * The stage of the console.
     */
    private Stage consoleStage;
    /**
     * A list of history commands.
     */
    private List<String> cmdList;
    /**
     * A helper index of {@link Console#cmdList}
     */
    private int index;
    /**
     * The text area where the commands and answers are displayed
     */
    private TextArea cmdDisplay;
    /**
     * The text field where client commands are typed
     */
    private TextField cmdField;
    /**
     * A vertical box layout to organize window elements.
     */
    private VBox vb;

    /**
     * Constructs a new console object and initilize the client attribute {@link client} 
     * with the parameter {@code c} and the list of commands {@link cmdList}. It also 
     * constucts the window elements and events.
     * @param c The client to usein the console.
     */
    public Console(Client c) {
        cmdList = new ArrayList<String>();
        index = -1;
        client = c;
        consoleStage = new Stage();
        consoleStage.setWidth(500);
        consoleStage.setHeight(400);
        consoleStage.setResizable(false);
        consoleStage.setTitle("Client console " + c.getId());
        cmdDisplay = new TextArea();
        cmdDisplay.setText("Here will appear the comunication\n");
        cmdDisplay.setEditable(false);
        cmdDisplay.setPrefSize(500, 350);
        cmdField = new TextField();
        cmdField.setPromptText("Enter your command here");
        cmdField.setPrefSize(500, 50);
        vb = new VBox(cmdDisplay, cmdField);

        cmdField.setOnKeyPressed(new EventHandler<KeyEvent>(){

            @Override
            public void handle(KeyEvent e) {
                if (e.getCode() == KeyCode.ENTER) {
                    onEnterClick();
                }
                else if (e.getCode() == KeyCode.UP && index >= 0) {
                    if (index != 0)
                        index--;
                    cmdField.setText(cmdList.get(index));
                }
                else if (e.getCode() == KeyCode.DOWN && index <= cmdList.size() - 1) {
                    if (index != cmdList.size() - 1)
                        index++;
                    cmdField.setText(cmdList.get(index));
                }
            }
            
        });
    }

    /**
     * Sets the console scene in the stage {@link Console#consoleStage} to {@code s}
     * @param s The scene to set.
     */
    public void setScene(Scene s) {
        consoleStage.setScene(s);
    }

    /**
     * Shows the console stage.
     */
    public void show() {
        consoleStage.show();
    }

    /**
     * Gets the root of the console window.
     * @return The root of the console window
     */
    public Parent getRoot() {
        return vb;
    }

    /**
     * This function is called when enter key is pressed, it uses the function {@link Client.communicate} to send the message typed in the console to the server, and diplays the answer.
     */
    private void onEnterClick() {
        String command = cmdField.getText();
        if (!command.equals("clear")) {
            cmdList.remove("");
            cmdList.add(command);
            cmdList.add("");
            index = cmdList.size() - 1;
            cmdField.clear();
            try {
                String response = client.communicate(command);
                cmdDisplay.setText(cmdDisplay.getText() + "> " + command + "\n");
                cmdDisplay.setText(cmdDisplay.getText() + "< " + response + "\n");
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        else {
            cmdField.clear();
            cmdDisplay.clear();
            cmdList.clear();
            index = -1;
        }
    }
    
    /**
     * Closes the console window.
     */
    public void close() {
        Stage s = (Stage) cmdDisplay.getScene().getWindow();
        s.close();
    }
}
