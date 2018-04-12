
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QKeyEvent>
#include <QMessageBox>
#include <QSound>
#include "MenuWindow.h"
#include "StartWindow.h"
#include "GameWindow.h"
#include "Games.h"


class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();
	~MainWindow();
	void keyPressEvent(QKeyEvent *event);
	void questionMenuSetup();

//Methods that should only be called by the constructor
protected:
	void constructorLogic();
	void creatingObjects();
	void connectSignals();
	void settingWidgets();
	void settingMainWindow();
	void settingSounds();
		
public slots:
	void deleteStart();
	void showMenuWindow();
	void gameWindow();
	void returnToMenu();
	void quitGame();
	void nextSong();
	void prevSong();
	void playPause();

	//GameFunctions
	void p1_chooseCharacter();
	void p1_askQuestion();
	//void p1_getAnswer();
	//void p1_answerQuestion(std::vector<int>);
	void p2_chooseCharacter();
	//void p2_askQuestion();
	void p2_answerQuestion(std::vector<int>);
	//void p2_answerQuestionYes();
	//void p2_answerQuestionNo();

signals:
	void escapeKeyPressed();
	void keyPressed();

private:
	int numberGames;
	int numberPlayer;
	int activeSong;
	bool secondPlayerIsBot;
	bool play;
	QString soundsFileExtension = ".wav";
	QString defaultSongPath = "./Sounds/"; //Default path of the sosund effect
	//Variables for storing the sound effects
	std::vector<QSound*> soundEffect;
	std::vector<string> soundEffectNames;
	//Variables for the game music
	std::vector<QSound*> songPlaylist;
	std::vector<QString> songNames;
	
	StartWindow *start;
	MenuWindow *menu;
	GameWindow *player1GameWindow;
	GameWindow *player2GameWindow;
	Games *gameLogic;
	QString player1Name;
	QString player2Name;	
	
	
};
	
#endif 