## Introduction
15 Puzzle Game Development Exercise

Using cocos2d-x-2.2.2

## Week 1
<img src="https://github.com/cliffzhaobupt/15Puzzle/raw/master/SampleGIF/basic_move.gif"/>

After entering the application, the user will see the puzzle boxes.

These puzzle boxes are randomly placed.

<img src="https://github.com/cliffzhaobupt/15Puzzle/raw/master/SampleGIF/finish_game.gif"/>

When user successfully finish the game, he'll see a promp.

If the user touch the screen now, new game will begin.

(For debugging, I have made the boxes' place fixed, not random)

## Week 2
<img src="https://github.com/cliffzhaobupt/15Puzzle/raw/master/SampleGIF/main_menu_and_ranking.gif"/>

Create tha main menu of the game.

The user can be redirected to the game and the ranking.

In the ranking page, the user can scroll the screen to see more.

I use a file in JSON format to store the local ranking.

TODO: I'll deal with the global ranking in the next week.

<img src="https://github.com/cliffzhaobupt/15Puzzle/raw/master/SampleGIF/finish_game_week_2.gif"/>

During the game, user can click Rstart Game button to play again.

After finishing the game, user can input his name, in order to add his name and the time he spent into the ranking.

Later, user can select to start another game, or to check the ranking

(For debugging, the game was set to be very simple...)

## Week 3
The server side code is saved in the directory called "RankingServer"

<img src="https://github.com/cliffzhaobupt/15Puzzle/raw/master/SampleGIF/global_ranking.gif"/>

In the ranking scene, user can touch Global Ranking tab to see global ranking.

After touching that tab, client will send a GET request to the server in order to acquire global ranking.

After response has arrived, global ranking will be displayed.

If the request failed, Network Error will be displayed.(In the GIF, I shut the server down...)

<img src="https://github.com/cliffzhaobupt/15Puzzle/raw/master/SampleGIF/finish_game_week_3.gif"/>

After the game has finished and user name has been entered, client will send a POST request to the server in order to add a new record.

As you can see in the GIF, both the Local Ranking and Global Ranking are updated.
