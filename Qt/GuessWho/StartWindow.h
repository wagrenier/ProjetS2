/****************************************
GuessWho ProjetS2 - APP7Gi

P14
William Adam-Grenier - adaw2602
Charles Quesnel - quec2502
Maxime St-Onge - stom2105

Avril 2018

StartWindow.h
*****************************************/
#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QWidget>
#include <QFont>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QPixmap>

class StartWindow : public QWidget
{
	Q_OBJECT

public:
	StartWindow(QWidget* parent);
	~StartWindow();

protected:
	void constructorLogic();
	void settingVariables();
	void creatingObjects();
	void settingObjectsProperties();
	void settingLayout();
	void settingWindow();

private:
	int widthImage;
	int heightImage;
	QPixmap *logoImage;
	QLabel * image;
	QLabel *quitButton;
	QVBoxLayout *layoutPrincipal;

};
#endif 
