#include <SFML/Graphics.hpp>
#include "Client_Manager.h"
#include <thread>
#include <Windows.h>
#include "Button.h"
#include "Game_Manager.h"
#include "GamePlay_Button.h"
#include <ctime>

sf::Event event;
std::queue<std::string> messages;
std::queue<std::string> msg_to_server;
std::queue<std::string> msg_from_server;
Client_Manager client_manager(&messages,&msg_to_server,&msg_from_server);
Game_Manager game_manager;


// Testing 
bool can_click_button = true;
int button_timer = 0;
bool Can_Play = false;

// Global msg string to shutdown everything
std::string global_msg = " ";

// GamePlay Button Display
std::vector<GamePlay_Button> Grid_Buttons;
std::vector<sf::RectangleShape> Grid_Lines;
std::vector<int> tictactoe{0,0,0,0,0,0,0,0,0};
int offsetX = 0;
int offsetY = 0;

// Game
int Turn;
std::string WhoWon_s;

int Translate_Message() {
    int temp_msg;

        temp_msg = std::stoi(msg_from_server.front()); 

        if (msg_from_server.front() == "") {
            client_manager.Close_Socket();
            client_manager.Switch_state(STATE::RESTING);
            game_manager.switch_state(GAMESTATE::MAINMENU);
            Can_Play = false;
        } // prevent bad data 
        else {
            switch (temp_msg)
            {
            case 1:
                if (Can_Play == true) {
                    Grid_Buttons.at(0).text.setString("O");
                    Grid_Buttons.at(0).text.setPosition(sf::Vector2f(Grid_Buttons.at(0).shape.getPosition().x, Grid_Buttons.at(0).shape.getPosition().y));
                    Grid_Buttons.at(0).can_be_clicked = false;
                    Turn++; // Push the turn Forward when we get data from server
                }
                break;
            case 2:
                if (Can_Play == true) {
                    Grid_Buttons.at(1).text.setString("O");
                    Grid_Buttons.at(1).text.setPosition(sf::Vector2f(Grid_Buttons.at(1).shape.getPosition().x, Grid_Buttons.at(1).shape.getPosition().y));
                    Grid_Buttons.at(1).can_be_clicked = false;
                    Turn++; // Push the turn Forward when we get data from server
                }
                break;
            case 3:
                if (Can_Play == true) {
                    Grid_Buttons.at(2).text.setString("O");
                    Grid_Buttons.at(2).text.setPosition(sf::Vector2f(Grid_Buttons.at(2).shape.getPosition().x, Grid_Buttons.at(2).shape.getPosition().y));
                    Grid_Buttons.at(2).can_be_clicked = false;
                    Turn++; // Push the turn Forward when we get data from server
                }
                break;
            case 4:
                if (Can_Play == true) {
                    Grid_Buttons.at(3).text.setString("O");
                    Grid_Buttons.at(3).text.setPosition(sf::Vector2f(Grid_Buttons.at(3).shape.getPosition().x, Grid_Buttons.at(3).shape.getPosition().y));
                    Grid_Buttons.at(3).can_be_clicked = false;
                    Turn++; // Push the turn Forward when we get data from server
                }
                break;
            case 5:
                if (Can_Play == true) {
                    Grid_Buttons.at(4).text.setString("O");
                    Grid_Buttons.at(4).text.setPosition(sf::Vector2f(Grid_Buttons.at(4).shape.getPosition().x, Grid_Buttons.at(4).shape.getPosition().y));
                    Grid_Buttons.at(4).can_be_clicked = false;
                    Turn++; // Push the turn Forward when we get data from server
                }
                break;
            case 6:
                if (Can_Play == true) {
                    Grid_Buttons.at(5).text.setString("O");
                    Grid_Buttons.at(5).text.setPosition(sf::Vector2f(Grid_Buttons.at(5).shape.getPosition().x, Grid_Buttons.at(5).shape.getPosition().y));
                    Grid_Buttons.at(5).can_be_clicked = false;
                    Turn++; // Push the turn Forward when we get data from server
                }
                break;
            case 7:
                if (Can_Play == true) {
                    Grid_Buttons.at(6).text.setString("O");
                    Grid_Buttons.at(6).text.setPosition(sf::Vector2f(Grid_Buttons.at(6).shape.getPosition().x, Grid_Buttons.at(6).shape.getPosition().y));
                    Grid_Buttons.at(6).can_be_clicked = false;
                    Turn++; // Push the turn Forward when we get data from server
                }
                break;
            case 8:
                if (Can_Play == true) {
                    Grid_Buttons.at(7).text.setString("O");
                    Grid_Buttons.at(7).text.setPosition(sf::Vector2f(Grid_Buttons.at(7).shape.getPosition().x, Grid_Buttons.at(7).shape.getPosition().y));
                    Grid_Buttons.at(7).can_be_clicked = false;
                    Turn++; // Push the turn Forward when we get data from server
                }
                break;
            case 9:
                if (Can_Play == true) {
                    Grid_Buttons.at(8).text.setString("O");
                    Grid_Buttons.at(8).text.setPosition(sf::Vector2f(Grid_Buttons.at(8).shape.getPosition().x, Grid_Buttons.at(8).shape.getPosition().y));
                    Grid_Buttons.at(8).can_be_clicked = false;
                    Turn++; // Push the turn Forward when we get data from server
                }
                break;
            case 10: // We have been Kicked from the server or the server has shut down
                client_manager.Close_Socket();
                client_manager.Switch_state(STATE::RESTING);
                game_manager.switch_state(GAMESTATE::MAINMENU);
                Can_Play = false;
                break;
            case 11:
                // WE were just send a cheack by the server to see if we are connected
                break;
            case 12:
                // Allow to Join Game
                // Make the Top Button usable
                Can_Play = true;
                break;

            case 13: // You Go first
                Turn = 1;
                break;

            case 14: // You Go Secand
                Turn = 2;
                break;
            case 15:
                // You Won 
                Turn = 4;
                WhoWon_s = "You Won";
                // Clean All Buttons
                break;
            case 16:
                // You lost
                Turn = 5;
                WhoWon_s = "You Lost";
                // Clean ALl Buttons
                break;
            }      
 
        }

        msg_from_server.pop();
        return 0;
}


int Client_Thread() {

    while (global_msg != "Quit") {
        switch (client_manager.state)
        {
        case STATE::RESTING:                                    // We do absolutly nothing here
                                                        break;

        case STATE::STARTCLIENT:                                // Start the Client
            client_manager.Set_Socket_Blocking(0);              // make the sokcet  blocking
            client_manager.Start_Client();     
            client_manager.Switch_state(STATE::RESTING);break;

        case STATE::LOOKINGFORCONNECTION:                       // Looking for connections
            client_manager.Set_Socket_Blocking(0);              // make the sokcet  blocking
            client_manager.LookingforConnection_state();
            if (client_manager.conected == true) {
                client_manager.Switch_state(STATE::HANDSHAKE); break;
            }
            else {
                client_manager.Switch_state(STATE::RESTING); break;
            }
            client_manager.Switch_state(STATE::RESTING); break;

        case STATE::RECIEVING:                                  // Recieving data
            client_manager.Set_Socket_Blocking(0);              // make the sokcet  blocking
            client_manager.Recive_State();
            client_manager.Switch_state(STATE::RESTING);break;

        case STATE::SENDING:                                    // Sending data
            client_manager.Set_Socket_Blocking(0);              // make the sokcet  blocking
            client_manager.Send_State();
            client_manager.Switch_state(STATE::RESTING);break;

        case STATE::HANDSHAKE:           
            client_manager.Handshake_state();
            break;

        case STATE::RECNONBLOCKING:
            client_manager.Set_Socket_Blocking(1);// make the sokcet non blocking
            client_manager.Recive_NB_state();   
            if (msg_from_server.empty() == false) {
                Translate_Message();
                break;
            }          

             break;
        }
    }
    return 0;
}


int main()
{
    // Multithreading
    //--------------------------------
    int Client_Thread();
    std::thread client_thread(Client_Thread);
    client_manager.Switch_state(STATE::STARTCLIENT);// start the client first
    game_manager.switch_state(GAMESTATE::MAINMENU); // make the game go into main menu

    // SFML window creatin
    //--------------------------------
    sf::RenderWindow window(sf::VideoMode(800,600), "TicTakToa");
    window.setFramerateLimit(60);

    // Texts
    //--------------------------------
    sf::Font font;
    sf::Text text,text2,text5,text6,text7,Who_Won_Text;


    // Loading the Font from folder
    if (!font.loadFromFile("./Resources/Pixellettersfull-BnJ5.ttf"))
    {
        // error...
        terminate();
    }

    // Client State Infrmation
    text.setFont(font);
    text.setCharacterSize(24);
    text.setString("");
    text.setFillColor(sf::Color::Black);
    text.setPosition(sf::Vector2f(500,575.f));

    // Server State RAW
    text2.setFont(font);
    text2.setCharacterSize(24);
    text2.setString("Server State : ");
    text2.setFillColor(sf::Color::Black);
    text2.setPosition(sf::Vector2f(300, 575.f));

    // Client ID 
    text5.setFont(font);
    text5.setCharacterSize(24);
    text5.setString("");
    text5.setFillColor(sf::Color::Black);
    text5.setPosition(sf::Vector2f(0, 575.f));

    // Connection Info
    text6.setFont(font);
    text6.setCharacterSize(24);
    text6.setString("Connection : ");
    text6.setFillColor(sf::Color::Black);
    text6.setPosition(sf::Vector2f(0, 0));

    // Turn Info
    text7.setFont(font);
    text7.setCharacterSize(24);
    text7.setString("Turn :  ");
    text7.setFillColor(sf::Color::Black);
    text7.setPosition(sf::Vector2f(0, 0));

    // Who Won Text
    Who_Won_Text.setFont(font);
    Who_Won_Text.setCharacterSize(24);
    Who_Won_Text.setString("Turn :  ");
    Who_Won_Text.setFillColor(sf::Color::Black);
    Who_Won_Text.setPosition(sf::Vector2f(0,50));


    // Colours
    //--------------------------------

    sf::Color background{ 179, 213, 255 };
    sf::Color background_tinted{ 150, 179, 214 };

    // Set up Game play Buttons
    //--------------------------------

    GamePlay_Button Game_button_temp(220, 120, 100, 100, &font, "0", background, background_tinted);
    Game_button_temp.text.setCharacterSize(34);

    // Seting up the Grid Lines
    sf::RectangleShape grid_line1, grid_line2, grid_line3, grid_line4;
    grid_line1.setSize(sf::Vector2f(10,360));
    grid_line1.setFillColor(sf::Color::Black);
    grid_line1.setPosition(sf::Vector2f(320.f, 100.f));

    grid_line2.setSize(sf::Vector2f(10, 360));
    grid_line2.setFillColor(sf::Color::Black);
    grid_line2.setPosition(sf::Vector2f(430.f, 100.f));

    grid_line3.setSize(sf::Vector2f(360, 10));
    grid_line3.setFillColor(sf::Color::Black);
    grid_line3.setPosition(sf::Vector2f(200, 220.f));

    grid_line4.setSize(sf::Vector2f(360, 10));
    grid_line4.setFillColor(sf::Color::Black);
    grid_line4.setPosition(sf::Vector2f(200.f, 330.f));

    for (int i = 0; i < 9; i++) {
        Grid_Buttons.push_back(Game_button_temp);
        Grid_Buttons.at(i).ID = i;
        Grid_Buttons.at(i).text.setString("");
        Grid_Buttons.at(i).shape.setPosition(sf::Vector2f(Grid_Buttons.at(i).shape.getPosition().x + offsetX, 
                                                          Grid_Buttons.at(i).shape.getPosition().y + offsetY));
        Grid_Buttons.at(i).ID = i; // sets the buttons ID
        offsetX += 110;
        if (i == 2) { 
            offsetY = 110; 
            offsetX = 0;
        };
        if (i == 5) { 
            offsetY = 220;
            offsetX = 0;
        }      
    }

    // Buttons
    //--------------------------------
    Button buton1(50, 100, 150, 100, &font, "Start Game",sf::Color::Blue,sf::Color::Green);           // Looking for game
    Button buton2(50, 220, 150, 100, &font, "Join Server", sf::Color::Blue, sf::Color::Green);      // Join server
    Button buton3(50, 340, 150, 100, &font, "Quit Game", sf::Color::Blue, sf::Color::Green);        // Quit Game

    // Test for timing
    //--------------------------------
    int time_test = 0;

    // Main Window Loop
    //--------------------------------
    while (window.isOpen())
    {       

        // Loop for cheacking events like Closing window
        //--------------------------------
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)               
                window.close();
        }

        
        switch (game_manager.state)
        {
        case GAMESTATE::MAINMENU: {
            // Button Stoper
            //--------------------------------
            if (button_timer == 90) { can_click_button = true; button_timer = 0; } // 60 = 1s

            // testing for displaying time
            //--------------------------------
            if (time_test == 60) { // 90 = 1.5s
                if (!messages.empty()) { text.setString(messages.front()); messages.pop(); }
                text2.setString("Server State : " + std::to_string(client_manager.state));
                time_test = 0;
            }

           


            // Mouse Position
            //--------------------------------
            sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
            sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

            // Update Start
            //--------------------------------
            window.clear(background);

            if (Can_Play == true) {
                buton1.render(&window); // Look for Game Button
                if (can_click_button == true) {
                    if (buton1.update(worldPos) == true) {

                        // Look for game state 
                        game_manager.switch_state(GAMESTATE::GAME); // Make this be "Looking For Game" where we setup the turn Order

                        // then will will switch to a game loop switch statment
                        can_click_button = false;
                    }
                }
            }

            buton2.render(&window); // Join Server
            if (can_click_button == true) {
                if (buton2.update(worldPos) == true) {
                    if (client_manager.conected != true) {
                        //msg_to_server.push(name);
                        client_manager.Switch_state(STATE::LOOKINGFORCONNECTION);
                        can_click_button = false;
                    }
                    else {
                        can_click_button = false;
                    }
                }
            }

            buton3.render(&window); //Quit game
            if (can_click_button == true) {
                if (buton3.update(worldPos) == true) {
                    window.close();
                    can_click_button = false;
                }
            }

            if (client_manager.conected == true) { text6.setString("Server Connection : Connected"); }
            else { text6.setString("Server Connection : No Connection"); }

            // Displaying and changing any info text
            text5.setString("Client ID : " + client_manager.Client_ID); // Display the Client ID
            window.draw(text6);
            window.draw(text);
            window.draw(text2);
            window.draw(text5);

            Who_Won_Text.setString(WhoWon_s);
            window.draw(Who_Won_Text);

            // Update End
            //--------------------------------
            window.display();

            // testing for diplaying time
            //--------------------------------
            time_test++;
            button_timer++;

            break; }
        case GAMESTATE::LOOKKINGFOROPONENT: {
            // Update Start
            window.clear(background);

            // Ask for Turn Order Here
            // Display who goes first and give Acknowlagmant

            // Update End
            window.display();

            break; }
        case GAMESTATE::GAME: {
            // Main Game     
           
            switch (Turn) {
            case 0: // Wait For a Message From Server
                WhoWon_s = "";
                break;

            case 1: {
                // Mouse Position
                //--------------------------------
                sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

                // Update Start
                window.clear(background);

                // Drawin the Clickable area
                for (int i = 0; i < Grid_Buttons.size(); i++) {
                    Grid_Buttons.at(i).render(&window);
                }

                //  Your Turn
                //  Cheacking if a client button was clicked 
                for (int i = 0; i < Grid_Buttons.size(); i++) {
                    if (Grid_Buttons.at(i).update(worldPos) == true) {

                        Grid_Buttons.at(i).text.setString("X");
                        Grid_Buttons.at(i).text.setPosition(sf::Vector2f(Grid_Buttons.at(i).shape.getPosition().x, Grid_Buttons.at(i).shape.getPosition().y));
                        Grid_Buttons.at(i).can_be_clicked = false;   // Block this Button

                        msg_to_server.push(std::to_string(i+1));      // push the button Counter in vector as the Position of which position of grid to update
                        client_manager.Switch_state(STATE::SENDING);
                        Turn++;                                       // Next Turn Wait for Data Back
                    }
                }

                    text7.setString("Turn : " + std::to_string(Turn));
                    // Drawin Grid Lines
                    window.draw(grid_line1);
                    window.draw(grid_line2);
                    window.draw(grid_line3);
                    window.draw(grid_line4);
                    window.draw(text7);

                    Who_Won_Text.setString(WhoWon_s);
                    window.draw(Who_Won_Text);


                    // Update End
                    window.display();

                    client_manager.Switch_state(STATE::RECNONBLOCKING);

                    break;}

            case 2: { // Wait for Data from Server

                // Update Start
                window.clear(background);

                // Drawin the  Buttons
                for (int i = 0; i < Grid_Buttons.size(); i++) {
                    Grid_Buttons.at(i).render(&window);
                }

                text7.setString("Turn : " + std::to_string(Turn));
                // Drawin Grid Lines
                window.draw(grid_line1);
                window.draw(grid_line2);
                window.draw(grid_line3);
                window.draw(grid_line4);
                window.draw(text7);

                Who_Won_Text.setString(WhoWon_s);
                window.draw(Who_Won_Text);

                // Update End
                window.display();

                client_manager.Switch_state(STATE::RECNONBLOCKING);

         
                break; }

            case 3: // Go back to Turn 1
                Turn = 1;
                break;

            case 4: // Win 
                Can_Play = false;
                // Clean The buttons
                for (int i = 0; i < Grid_Buttons.size(); i++) {
                    Grid_Buttons.at(i).text.setString("");
                    Grid_Buttons.at(i).can_be_clicked = true;
                }
                game_manager.switch_state(GAMESTATE::MAINMENU);
                Turn = 0;
                break;
            case 5: // Lost
                Can_Play = false;
                // Clean The buttons
                for (int i = 0; i < Grid_Buttons.size(); i++) {
                    Grid_Buttons.at(i).text.setString("");
                    Grid_Buttons.at(i).can_be_clicked = true;
                }
                game_manager.switch_state(GAMESTATE::MAINMENU);
                Turn = 0;
                break;
            }

            break; 
            }
        }

    }

    // Clean UP
    global_msg = "Quit"; // for now to close the thread
    client_thread.join();
    client_manager.Client_cleanup();
    return 0;
}