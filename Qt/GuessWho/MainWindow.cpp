#include "MainWindow.h"

MainWindow::MainWindow() : QMainWindow()
{
	constructorLogic();
}

MainWindow::~MainWindow()
{
	delete menu;
}

void MainWindow::constructorLogic()
{
	settingMainWindow();
	creatingObjects();
	connectSignals();
	settingWidgets();
}

void MainWindow::creatingObjects()
{
	//Creating the objects for the GUI and Game
	start = new StartWindow(this);
	gameLogic = new Games();
	menu = new MenuWindow(this, gameLogic->get_character_manager());
	//Songs
	electroCabello = new QSound("./Sounds/Electro_Cabello.wav", this);
	epicSaxGuy = new QSound("./Sounds/epicsaxguy.wav", this);
	luckyDay = new QSound("./Sounds/Jingle-Punks-Lucky-Day.wav", this);
	weAreOne = new QSound("./Sounds/Vexento-WeAreOne.wav", this);
	adventures = new QSound("./Sounds/adventures-A-Himitsu.wav", this);
	summer = new QSound("./Sounds/bensound-summer.wav", this);
	carefree = new QSound("./Sounds/Kevin_MacLeod_-_Carefree.wav", this);
	electroCabello->setLoops(QSound::Infinite);
	epicSaxGuy->setLoops(QSound::Infinite);
	luckyDay->setLoops(QSound::Infinite);
	weAreOne->setLoops(QSound::Infinite);
	adventures->setLoops(QSound::Infinite);
	summer->setLoops(QSound::Infinite);
	carefree->setLoops(QSound::Infinite);
	activeSong = "electroCabello";
	electroCabello->play();
	//Loading the default character list
	gameLogic->get_character_manager()->importCharacters
	(menu->getOptionsMenu()->getActiveList().toStdString());
}

void MainWindow::connectSignals()
{
	//Connecting the signals
	connect(this, SIGNAL(keyPressed()), this, SLOT(deleteStart()));
	connect(menu->getNextButton(), SIGNAL(clicked()), this, SLOT(nextSong()));
	connect(menu->getPrevButton(), SIGNAL(clicked()), this, SLOT(prevSong()));
}

void MainWindow::settingWidgets()
{
	//Setting the widgets
	setCentralWidget(start);
	menu->hide();
}

void MainWindow::settingMainWindow()
{
	//Setting the MainWindow
	setWindowTitle("Guess Who?");
	setStyleSheet("background-image: url(./Photos/header_logo.png)");
	this->setGeometry(100, 100, 1366, 768);
	//showFullScreen();
	show();
}

void MainWindow::deleteStart()
{
	disconnect(this, SIGNAL(keyPressed()), this, SLOT(deleteStart()));
	start->close();
	delete start;
	connect(menu->getOkOnePlayerButton(), SIGNAL(clicked()), this, SLOT(gameWindow()));
	connect(menu->getOkTwoPlayersButton(), SIGNAL(clicked()), this, SLOT(gameWindow()));
	showMenuWindow();
}

void MainWindow::showMenuWindow()
{
	menu->show();
	setCentralWidget(menu);
	menu->showMainMenu();
}

void MainWindow::gameWindow()
{
	//GAME LOGIC *******************
	
	//Checks if enough characters in the vector if not -> error
	if (gameLogic->get_character_manager()->get_character_vector().size() < 20) {
		QMessageBox::critical(NULL, "Erreur", "Il n'y a pas assez de personnages dans la liste (minimum 20)", QMessageBox::Ok);
		menu->showMainMenu();
		return;
	}

	gameLogic->get_character_manager()->shuffleCharacters();
	gameLogic->get_character_manager()->printProperties();

	//This line is crucial, because it allows the other character manger to know what is the order
	cout << gameLogic->get_character_manager()->exportCharacters(menu->getOptionsMenu()->getActiveList().toStdString());
	//gameLogic->get_character_manager().printProperties();
	gameLogic->copyCharacterManagerToPlayer(gameLogic->getPlayer1(), menu->getOptionsMenu()->getActiveList().toStdString());
	gameLogic->copyCharacterManagerToPlayer(gameLogic->getPlayer2(), menu->getOptionsMenu()->getActiveList().toStdString());
	
	//gameLogic->getPlayer1Reference()->set_name_of_player(menu->);


	//GUI ************************
	menu->hide();
	takeCentralWidget();

	player1GameWindow = new GameWindow(this, gameLogic->getPlayer1Reference());
	player2GameWindow = new GameWindow(this, gameLogic->getPlayer2Reference());
	
	connect(this, SIGNAL(wKeyPressed()), player1GameWindow->getSideMenu()->getQuestionMenuBar(), SLOT(goUpTreeWidgetItem()));
	connect(this, SIGNAL(sKeyPressed()), player1GameWindow->getSideMenu()->getQuestionMenuBar(), SLOT(goDownTreeWidgetItem()));
	//connect(this, SIGNAL(enterKeyPressed()), player1GameWindow->getSideMenu()->getQuestionMenuBar(), SLOT(treeWidgetItemClicked()));

	//Connects the pause menu buttons
	connect(this, SIGNAL(escapeKeyPressed()), player1GameWindow, SLOT(togglePauseMenu()));
	connect(player1GameWindow->getPauseMenu(), SIGNAL(escapeKeyPressed()), player1GameWindow, SLOT(togglePauseMenu()));
	numberGames = menu->getNumberGames();
	player1Name = menu->getPlayer1Name();

	if (menu->getNumberPlayers() == 1) { //Second player is AI 
		player1Name = menu->getPlayer1Name();
		player2Name = "BOT";
		secondPlayerIsBot = true;
		gameLogic->getPlayer2Reference()->set_is_cpu(true);
	}
	else {
		player2Name = menu->getPlayer2Name();
		secondPlayerIsBot = false;
	}

	p1_chooseCharacter();
	disconnect(this, SIGNAL(escapeKeyPressed()), menu, SLOT(showMainMenu()));
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
	emit keyPressed();
	if (event->key() == Qt::Key_Escape) {
		emit escapeKeyPressed();
	}
	if(event->key() == Qt::Key_W)
	{
		emit wKeyPressed();
	}
	if (event->key() == Qt::Key_S)
	{
		emit sKeyPressed();
	}
	/*
	if (event->key() == Qt::Key_Enter)
	{
		cout << "Hey there!" << endl;
		emit enterKeyPressed();
	}*/
}

void MainWindow::returnToMenu() {
	this->player1GameWindow->getPauseMenu()->hide();
	int answer = QMessageBox::question(NULL, "Quitter la partie", "Ceci entraînera la fin de la partie, voulez-vous vraiment quitter?", QMessageBox::Yes | QMessageBox::No);
	if (answer == QMessageBox::Yes) { //Yes
		disconnect(this, SIGNAL(escapeKeyPressed()), player1GameWindow, SLOT(togglePauseMenu()));
		disconnect(player1GameWindow->getPauseMenu(), SIGNAL(escapeKeyPressed()), player1GameWindow, SLOT(togglePauseMenu()));

		this->player1GameWindow->getPauseMenu()->close();
		this->player1GameWindow->close();
		delete player1GameWindow;
		this->player2GameWindow->getPauseMenu()->close();
		this->player2GameWindow->close();
		delete player2GameWindow;
		this->showMenuWindow();
	}
	else { //No
		this->player1GameWindow->getPauseMenu()->show();
	}
}

void MainWindow::quitGame() {
	this->player1GameWindow->getPauseMenu()->hide();
	int answer = QMessageBox::question(NULL, "Quitter la partie", "Voulez-vous vraiment quitter la partie", QMessageBox::Yes | QMessageBox::No);
	if (answer == QMessageBox::Yes) { //Yes
		this->close();
		this->player1GameWindow->getPauseMenu()->close();
	}
	else { //No
		this->player1GameWindow->getPauseMenu()->show();
	}
}

void MainWindow::nextSong()
{
	if (activeSong == "electroCabello")
	{
		electroCabello->stop();
		epicSaxGuy->play();
		activeSong = "epicSaxGuy";
	}
	else if (activeSong == "epicSaxGuy")
	{
		epicSaxGuy->stop();
		luckyDay->play();
		activeSong = "luckyDay";
	}
	else if (activeSong == "luckyDay")
	{
		luckyDay->stop();
		weAreOne->play();
		activeSong = "weAreOne";
	}
	else if (activeSong == "weAreOne")
	{
		weAreOne->stop();
		adventures->play();
		activeSong = "adventures";
	}
	else if (activeSong == "adventures")
	{
		adventures->stop();
		summer->play();
		activeSong = "summer";
	}
	else if (activeSong == "summer")
	{
		summer->stop();
		carefree->play();
		activeSong = "carefree";
	}
	else if (activeSong == "carefree")
	{
		carefree->stop();
		electroCabello->play();
		activeSong = "electroCabello";
	}
}
	

void  MainWindow::prevSong()
{
	if (activeSong == "electroCabello")
	{
		electroCabello->stop();
		carefree->play();
		activeSong = "carefree";
	}
	else if (activeSong == "carefree")
	{
		carefree->stop();
		summer->play();
		activeSong = "summer";
	}
	else if (activeSong == "summer")
	{
		summer->stop();
		adventures->play();
		activeSong = "adventures";
	}
	else if (activeSong == "adventures")
	{
		adventures->stop();
		weAreOne->play();
		activeSong = "weAreOne";
	}
	else if (activeSong == "weAreOne")
	{
		weAreOne->stop();
		luckyDay->play();
		activeSong = "luckyDay";
	}
	else if (activeSong == "luckyDay")
	{
		luckyDay->stop();
		epicSaxGuy->play();
		activeSong = "epicSaxGuy";
	}
	else if (activeSong == "epicSaxGuy")
	{
		epicSaxGuy->stop();
		electroCabello->play();
		activeSong = "electroCabello";
	}
}

void MainWindow::questionMenuSetup()
{
	player1GameWindow->getSideMenu()->getQuestionMenuBar();
}

void MainWindow::p1_chooseCharacter() {
	setCentralWidget(player1GameWindow);

	player1GameWindow->getLowerBar()->changeText(player1Name.toStdString() + " : Veuillez choisir votre personnage", OK_MODE);
	player1GameWindow->toggleSelectMode();
	connect(player1GameWindow->getLowerBar()->getOkButton(), SIGNAL(clicked()), this, SLOT(p2_chooseCharacter()));
}

void MainWindow::p2_chooseCharacter() {
	player1GameWindow->toggleSelectMode(); //Resets to normal mode
	player2GameWindow->toggleSelectMode(); //Sets to select mode

	if (secondPlayerIsBot) {
		gameLogic->getPlayer2Reference()->generateRandomCharacter();
		p1_askQuestion();
	}
	else {
		player1GameWindow->hide();
		takeCentralWidget();
		setCentralWidget(player2GameWindow);

		player2GameWindow->getLowerBar()->changeText(player2Name.toStdString() + " : Veuillez choisir votre personnage", OK_MODE);
		connect(player2GameWindow->getLowerBar()->getOkButton(), SIGNAL(clicked()), this, SLOT(p1_askQuestion()));
	}
}

void MainWindow::p1_askQuestion() {
	if (!secondPlayerIsBot) {
		player2GameWindow->hide();
		takeCentralWidget();
		setCentralWidget(player1GameWindow);
	}

	if(player2GameWindow->getSelectMode() == true)
		player2GameWindow->toggleSelectMode(); //Resets to normal mode

	player1GameWindow->getLowerBar()->changeText(player1Name.toStdString() + " : Veuillez poser votre question", EMPTY_MODE);

	//Connect question onClick() to p2_answerQuestion(vector)
	//Only for testing
	std::vector<int> q;
	q.push_back(0);
	q.push_back(0);
	p2_answerQuestion(q);
}

void MainWindow::p2_answerQuestion(std::vector<int> q) {
	if (secondPlayerIsBot) {
		bool answer = gameLogic->getAnswerToQuestion(q.at(0), q.at(1), gameLogic->getPlayer1Reference(), gameLogic->getPlayer2Reference());
		cout << answer << endl;
	}
	else {
		player1GameWindow->hide();
		takeCentralWidget();
		setCentralWidget(player2GameWindow);

		player2GameWindow->getLowerBar()->changeText(player2Name.toStdString() + gameLogic->convertQuestionToString(q.at(0), q.at(1)), YES_NO_MODE);
		//connect(player2GameWindow->getLowerBar()->getYesButton(), SIGNAL(clicked()), this, SLOT(p2_answerQuestionYes()));
		//connect(player2GameWindow->getLowerBar()->getNoButton(), SIGNAL(clicked()), this, SLOT(p2_answerQuestionNo()));
	}
}
