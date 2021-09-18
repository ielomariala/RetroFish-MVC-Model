public class Fish{
    /**
     * The name of th Fish, it final because it cannot be changed
     */
    private final String name;

    /**
     * The objective coordinates of the fish, it have to reach them after {@code time} second
     */
    private double x_obj;
    private double y_obj;

    /**
     * The Current coodinates of the fish.
     */
    private double x_pos;
    private double y_pos;

    /**
     * The previous coordinates of the fish, where the fish was before receiving the new coodinates.
     */
    private double x_prev;
    private double y_prev;

    /**
     * The size of the fish.
     */
    private int width;
    private int height;

    /**
     * The time that passed after the creation of the this fish.
     */
    private double currTime;

    /**
     * The time that the fish has to reach the objective coordinates
     */
    private double time;

    /**
     * This function is a getter of the attribute {@code name}
     * @return Returns the {@code name} of the fish as a {@code String}
     */
    public String getName(){
        return name;
    }

    /**
     * This function is a getter of the attribute {@code x_pos}
     * @return Returns the {@code x_pos} of the fish
     */
    public double getXpos(){
        return x_pos;
    }

    /**
     * This function is a getter of the attribute {@code y_pos}
     * @return Returns the {@code y_pos} of the fish
     */
    public double getYpos(){
        return y_pos;
    }

    /**
     * This function is a getter of the attribute {@code x_obj}
     * @return Returns the {@code x_obj} of the fish
     */
    public double getObjX() {
        return x_obj;
    }

    /**
     * This function is a getter of the attribute {@code y_obj}
     * @return Returns the {@code y_obj} of the fish
     */
    public double getObjY() {
        return y_obj;
    }

    /**
     * Updates the position of the fish depending on the time
     * @param FPS Frames per second
     */
    public void updatePos(int FPS) {
        if (time != 0){
            currTime += 1. / (float) FPS;
            x_pos = x_prev + currTime * (x_obj - x_prev)/ time;
            y_pos = y_prev + currTime * (y_obj - y_prev)/ time;
        }
        else{
            x_prev = x_obj;
            y_prev = y_obj;
            x_pos = x_obj;
            y_pos = y_obj;
        }
    }

    /**
     * This function is a getter of tha attribute {@code width}
     * @return return {@code width} of the fish
     */
    public int getWidth(){
        return width;
    }

    /**
     * This function is a getter of tha attribute {@code height}
     * @return return {@code height} of the fish
     */
    public int getHeight(){
        return height;
    }

    /**
     * This function is a getter of tha attribute {@code time}
     * @return return {@code time} of the fish
     */
    public double getTime(){
        return time;
    }

    /**
     * Construct a new fish object and initialize its attributes
     * @param name The name of the new fish
     * @param x The x coordinate where the fish should be after {@code time} second
     * @param y The y coordinate where the fish should be after {@code time} second
     * @param x_prev The previous x coordinate where the fish was before receiving the new coodinates
     * @param y_prev The previous y coordinate where the fish was before receiving the new coodinates
     * @param width The width of the fish
     * @param height The height of the fish
     * @param time How much time the fish have to pass before reaching the {@code x} and {@code y}
     */
    public Fish(String name, double x, double y, double x_prev, double y_prev, int width, int height, double time){
        this.name = name;
        this.width = width;
        this.height = height;
        this.time = time;
        x_obj = x;
        y_obj = y;
        currTime = 0;
            if (x_prev == -1 && y_prev == -1) {
                this.x_prev = x;
                this.y_prev = y;
                x_pos = x;
                y_pos = y;
            }
            else {
                x_pos = x_prev;
                y_pos = y_prev;
                this.x_prev = x_prev;
                this.y_prev = y_prev;
            }
    }

    /**
     * Construct a new fish object and initialize its attributes
     * @param name The name of the new fish
     * @param x The x coordinate where the fish should be after {@code time} second
     * @param y The y coordinate where the fish should be after {@code time} second
     * @param width The width of the fish
     * @param height The height of the fish
     * @param time How much time the fish have to pass before reaching the {@code x} and {@code y}
     */
    public Fish(String name, double x, double y, int width, int height, double time) {
        this(name, x, y, -1, -1, width, height, time);
    }

    @Override
    public boolean equals(Object o) {
        Fish f = (Fish) o;
        return name.equals(f.name);
    }
}