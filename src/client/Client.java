import java.net.*;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.io.*;
import java.util.ArrayList;
import java.util.List;
import java.util.NoSuchElementException;
import java.util.Properties;
import java.util.Scanner;
import java.util.StringTokenizer;
import java.util.concurrent.Semaphore;

public class Client {
    /**
     * The port to connect to socket
     */
    private int port;
    /**
     * The address where the client should connect
     */
    private String controller_adress;
    /**
     * The id of the view where the client is connected
     */
    private String view_id;
    /**
     * The timeout value of the client
     */
    private int display_timeout_value;
    /**
     * The path to ressources used by the client
     */
    private String ressources;

    /**
     * The socket object
     */
    private Socket socket;
    /**
     * The reader of the socket
     */
    private BufferedReader in;
    /**
     * The writer of the socket
     */
    private PrintWriter out;

    /**
     * Log file object
     */
    private File logFile;

    /**
     * List of the fishes currently in the view with id {@code view_id}
     * @see Fish
     */
    private List<Fish> fishList;

    /**
     * A boolean that means that the connection has failed
     */
    private boolean cnxFailed = false;
    /**
     * A boolean that means that a client is disconnected from the socket
     */
    private boolean disconnected = false;

    /**
     * A semaphore object to synchronize Threads
     */
    private Semaphore sem;

    /**
     * Constucts a client object, this function connects the client to the server with
     * {@code controller_address} address and creates the needed threads for the connection.
     * @param configPath The path of configuration file.
     * @throws IOException IOException is thrown if file couldn't be open.
     */
    public Client(String configPath) throws IOException {
        Properties prop = new Properties();
        fishList = new ArrayList<Fish>();
        sem = new Semaphore(1);

        try {
            InputStream conf = new FileInputStream(configPath);
            prop.load(conf);
            conf.close();

            controller_adress = prop.getProperty("controller-address");
            view_id = prop.getProperty("id");
            port = Integer.parseInt(prop.getProperty("controller-port"));
            display_timeout_value = Integer.parseInt(prop.getProperty("display-timeout-value"));
            ressources = prop.getProperty("resources");
            
            socket = new Socket(controller_adress,port);
            in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            out = new PrintWriter(socket.getOutputStream(),true);
            sem.acquire();

            out.print("hello in as " + view_id);
            out.flush();

            Scanner sc = new Scanner(readLines(in));
            sc.next();
            view_id = sc.next();
            sc.close();

            sem.release();

            logFile = new File("../", "client_" + view_id + ".log");
            FileWriter fw = new FileWriter(logFile);
            PrintWriter pw = new PrintWriter(fw);
            pw.println("<<< Configuration >>>");
            pw.println("< controller address = " + controller_adress + " >");
            pw.println("< view id = " + view_id + " >");
            pw.println("< port = " + port + " >");
            pw.println("< display timeout value = " + display_timeout_value + " >");
            pw.println("< vontroller address = " + controller_adress + " >");
            pw.close();
            fw.close();
        }
        catch(ConnectException e) {
            System.out.println("Server isn't available or port isn't correct... Please reconnect");
            System.exit(1);
        }
        catch(NoSuchElementException e) {
            System.out.println("Couldn't scan view id from server");
            System.exit(1);
        }
        catch(Exception e) {
            e.printStackTrace();
        }
            
        Thread pingTh = new Thread(new Runnable(){
            @Override
            public void run() {
                try {
                    while (true) {
                        Thread.sleep(1000 * display_timeout_value);
                        while (cnxFailed) {
                            if (disconnected) {
                                break;
                            }
                            Thread.sleep(5000);
                        }
                        if (disconnected) {
                            break;
                        }
                        String pong = null;
                        sem.acquire();
                        FileWriter fw = new FileWriter(logFile, true);
                        PrintWriter pw = new PrintWriter(fw);
                        DateTimeFormatter dtf = DateTimeFormatter.ofPattern("uuuu/MM/dd HH:mm:ss");
                        LocalDateTime date = LocalDateTime.now();
                        pw.println("</ " + dtf.format(date));
                        pw.println("\tClient : {\n\t\tping 123\n\t}");

                        out.print("ping 123");
                        out.flush();

                        pong = readLines(in);

                        pw.println("\tServer : {\n\t\t" + pong + "\n}\n/>");
                        pw.close();
                        fw.close();

                        sem.release();
                        if (!pong.equals("pong 123")) {
                            disconnected = true;
                        }
                    }
                    communicate("disconnect");
                }
                catch (Exception e) {
                    e.printStackTrace();
                }
                    
            }
        });

        Thread getFishThread = new Thread(new Runnable(){

            @Override
            public void run() {
                try {
                    while(!disconnected) {
                        while (cnxFailed) {
                            if (disconnected) {
                                break;
                            }
                            Thread.sleep(5000);
                        }
                        String response = null;
                        sem.acquire();

                        out.print("getFishes");
                        out.flush();
                        response = readLines(in);
                        parseFishes(response);

                        sem.release();
                        Thread.sleep(1000);
                    }
                } catch(Exception e) {
                    e.printStackTrace();
                }
            }
        });
        pingTh.start();
        getFishThread.start();
    }

    /**
     * Closes the client's reader, writer and socket.
     * @throws IOException if an I/O error occurs when closing.
     */
    public void close() throws IOException {
        socket.close();
        in.close();
        out.close();
    }

    /**
     * A getter of the attribute {@code view_id}
     * @return the value of {@code view_id}
     */
    public String getId() {
        return view_id;
    }

    /**
     * This function sends the message {@code command} to server and returns the answer.
     * @param command The command to send to the server
     * @return The answer the server
     * @throws IOException if an I/O error occurs.
     */
    public String communicate(String command) throws IOException {
        String serverResponse;
        FileWriter fw = new FileWriter(logFile, true);
        PrintWriter pw = new PrintWriter(fw);
        DateTimeFormatter dtf = DateTimeFormatter.ofPattern("uuuu/MM/dd HH:mm:ss");
        LocalDateTime date = LocalDateTime.now();
        pw.println("</ " + dtf.format(date));
        pw.println("\tClient : {\n\t\t" + command + "\n\t}");
        
        if (command.split(" ")[0].equals("hello") && !disconnected) {
            cnxFailed = false;
            try {
                sem.acquire();
            } catch (Exception e) {
                e.printStackTrace();
            }
            out.print(command);
            out.flush();
            serverResponse = readLines(in);

            Scanner sc = new Scanner(serverResponse);
            sc.next();
            view_id = sc.next();
            sc.close();

            sem.release();
            pw.println("\tServer : {\n\t\t" + serverResponse + "\n}\n/>");
            pw.close();
            fw.close();
            return serverResponse;
        }
        if (disconnected) {
            pw.close();
            fw.close();
            return "You are disconnected";
        }
        if (cnxFailed) {
            pw.close();
            fw.close();
            return "You are logged out, please reconnect to a view to continue";
        }

        try {
            sem.acquire();
        } catch (Exception e) {
            e.printStackTrace();
        }
        out.print(command);
        out.flush();
        serverResponse = readLines(in);

        sem.release();

        pw.println("\tServer : {\n\t\t" + serverResponse + "\n}\n/>");
        pw.close();
        fw.close();

        if (command.equals("disconnect")) {
            disconnected = true;
            close();
        }
        if (command.equals("log out")) {
            cnxFailed = true;
        }

        return serverResponse;
    }

    /**
     * A getter of the fish list {@link Client#fishList}
     * @return The list of fishes currently in the view.
     * @see Fish
     */
    public List<Fish> getFishs() {
        return fishList;
    }

    /**
     * A getter of {@link Client#ressources} attribute
     * @return The ressources path.
     */
    public String getResources() {
        return ressources;
    }

    /**
     * Reads multiple lines from the buffered reader {@code in}
     * @param in The reader from where we read the lines
     * @return The lines read
     * @throws IOException if an I/O error occurs.
     */
    private String readLines(BufferedReader in) throws IOException {
        in.readLine();
        String serverResponse = "";
        String line = in.readLine();
        String newLine = "\n";
        while (!"}".equals(line) && line != null) {
            String lineToAdd = line;
            line = in.readLine();
            if ("}".equals(line))
                newLine = "";
            serverResponse += lineToAdd + newLine;
        }
        return serverResponse;
    }

    /**
     * Parses the message {@code msg} given as argument to fill fish list {@link Client#fishList}
     * @param msg The message given from server to parse.
     * @see Fish
     */
    private void parseFishes(String msg){
        ArrayList<Fish> tmpList = new ArrayList<Fish>(fishList);
        fishList.clear();
        if (!msg.isEmpty()) {
            StringTokenizer sst = new StringTokenizer(msg, "[");
            int count = sst.countTokens();
            sst.nextToken();
            count--;
            for (int i = 0; i < count; i++) {
                StringTokenizer st1 = new StringTokenizer(sst.nextToken(), " ");
                String name = st1.nextToken();
                st1.nextToken();
                String coord = st1.nextToken();
                String size = st1.nextToken();
                StringTokenizer st2 = new StringTokenizer(coord, ",");
                StringTokenizer st3 = new StringTokenizer(size, ",");
                String pos = st2.nextToken();
                String widthHeight = st3.nextToken();
                String timeStr = st1.nextToken();
                StringTokenizer pos_token = new StringTokenizer(pos, "x");
                StringTokenizer size_token = new StringTokenizer(widthHeight, "x");
                StringTokenizer time_token = new StringTokenizer(timeStr, ",");
                double posX = Double.parseDouble(pos_token.nextToken());
                double posY = Double.parseDouble(pos_token.nextToken());
                int width = Integer.parseInt(size_token.nextToken());
                int height = Integer.parseInt(size_token.nextToken());
                int time = Integer.parseInt(time_token.nextToken());
                Fish newFish = null;
                if (tmpList.isEmpty()) {
                    newFish = new Fish(name, posX, posY, width, height, time);
                }
                else {
                    for (Fish f : tmpList) {
                        if (name.equals(f.getName())) {
                            if ( f.getTime() != 0 && time > f.getTime() ){
                                newFish = new Fish(name, posX, posY, f.getObjX(), f.getObjY(), width, height, time);
                            }
                            else {
                                newFish = new Fish(name, posX, posY, f.getXpos(), f.getYpos(), width, height, time);
                            }
                            break;
                        }
                        else {
                            newFish = new Fish(name, posX, posY, width, height, time);
                        }
                    }
                }
                if (time == 0)
                    tmpList.add(newFish);
                fishList.add(newFish);
            }
        }
    }
}
