#include "GameWindow.h"



GameWindow::GameWindow(QWidget *parent):QWidget(parent)
{
	//setStyleSheet("background-image: url(./Photos/header_logo.png)");
	layout = new QGridLayout(this);
	grid = new CharacterGrid(this, parent->height()-100, parent->width());
	lowerBar = new LowerBar(this, parent->width());
	sideMenu = new SideMenu(this, parent->height());
	layout->addWidget(grid, 0, 0);
	layout->addWidget(lowerBar, 1, 0);
	layout->addWidget(sideMenu, 0, 1, 2, 1);
	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);
	//layout->setRowStretch(0, 90);
	//layout->setRowStretch(1, 10);

	//layout->setColumnStretch(0, 80);
	//layout->setColumnStretch(1, 20);

	this->setLayout(layout);
}


GameWindow::~GameWindow()
{
}
