#include "StatusBar.hpp"

#include <QHBoxLayout>
#include <QToolButton>
#include <QLabel>


StatusBar::StatusBar(QWidget* parent) : QStatusBar(parent)
{
	this->setMaximumHeight(22);

	QSizePolicy sizePolicy1(QSizePolicy::Policy::Expanding, QSizePolicy::Fixed);
	sizePolicy1.setHorizontalStretch(1);
	sizePolicy1.setVerticalStretch(0);
	sizePolicy1.setHeightForWidth(this->sizePolicy().hasHeightForWidth());

	m_pStatusWidget = new QWidget(this);

	sizePolicy1.setHeightForWidth(m_pStatusWidget->sizePolicy().hasHeightForWidth());

	m_pStatusWidget->setSizePolicy(sizePolicy1);
	m_pStatusWidget->setMaximumSize(QSize(16777215, 20));

	m_pStatusWidget->setStyleSheet(QString(R"(
		QWidget {
			border: none;
		}

		QToolButton {
			border: 1px solid rgb(232, 232, 232);
		}

		QLabel {
			border: 1px solid rgb(232, 232, 232);
		}
	)"));

	auto layout = new QHBoxLayout(m_pStatusWidget);
	layout->setSpacing(1);
	layout->setContentsMargins(1, 0, 2, 1);

	m_pOptionsBtn = new QToolButton(m_pStatusWidget);
	m_pOptionsBtn->setIconSize(QSize(12, 12));

	layout->addWidget(m_pOptionsBtn);

	m_pOptionGrip = new QToolButton(m_pStatusWidget);

	QIcon icon22;
	icon22.addFile(QString::fromUtf8(":/icons/min.png"), QSize(), QIcon::Normal, QIcon::Off);
	m_pOptionGrip->setIcon(icon22);
	m_pOptionGrip->setIconSize(QSize(12, 12));

	layout->addWidget(m_pOptionGrip);

	m_pMacroBtn = new QToolButton(m_pStatusWidget);
	layout->addWidget(m_pMacroBtn);

	m_pMacroGrip = new QToolButton(m_pStatusWidget);
	m_pMacroGrip->setIcon(icon22);
	m_pMacroGrip->setIconSize(QSize(12, 12));

	layout->addWidget(m_pMacroGrip);

	m_pLogBtn = new QToolButton(m_pStatusWidget);

	layout->addWidget(m_pLogBtn);

	m_pStatBtn = new QToolButton(m_pStatusWidget);

	layout->addWidget(m_pStatBtn);

	m_pBreakBtn = new QToolButton(m_pStatusWidget);

	layout->addWidget(m_pBreakBtn);

	m_pStatusLbl = new QLabel(m_pStatusWidget);

	QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Preferred);
	sizePolicy3.setHorizontalStretch(0);
	sizePolicy3.setVerticalStretch(0);
	sizePolicy3.setHeightForWidth(m_pStatusLbl->sizePolicy().hasHeightForWidth());
	m_pStatusLbl->setSizePolicy(sizePolicy3);

	layout->addWidget(m_pStatusLbl);

	m_pStatusText = new QLabel(m_pStatusWidget);

	layout->addWidget(m_pStatusText);

	m_pStatSelected = new QLabel(m_pStatusWidget);

	QSizePolicy sizePolicy4(QSizePolicy::Maximum, QSizePolicy::Preferred);
	sizePolicy4.setHorizontalStretch(0);
	sizePolicy4.setVerticalStretch(0);
	sizePolicy4.setHeightForWidth(m_pStatSelected->sizePolicy().hasHeightForWidth());
	m_pStatSelected->setSizePolicy(sizePolicy4);
	m_pStatSelected->setMinimumSize(QSize(90, 0));
	m_pStatSelected->setMaximumSize(QSize(90, 16777215));

	layout->addWidget(m_pStatSelected);

	m_pStatCoords = new QLabel(m_pStatusWidget);

	sizePolicy4.setHeightForWidth(m_pStatCoords->sizePolicy().hasHeightForWidth());
	m_pStatCoords->setSizePolicy(sizePolicy4);
	m_pStatCoords->setMinimumSize(QSize(120, 0));
	m_pStatCoords->setMaximumSize(QSize(120, 16777215));

	layout->addWidget(m_pStatCoords);

	m_pStatScreen = new QLabel(m_pStatusWidget);
	sizePolicy4.setHeightForWidth(m_pStatScreen->sizePolicy().hasHeightForWidth());
	m_pStatScreen->setSizePolicy(sizePolicy4);
	m_pStatScreen->setMinimumSize(QSize(90, 0));
	m_pStatScreen->setMaximumSize(QSize(90, 16777215));

	layout->addWidget(m_pStatScreen);

	m_pStatGrid = new QLabel(m_pStatusWidget);
	sizePolicy4.setHeightForWidth(m_pStatGrid->sizePolicy().hasHeightForWidth());
	m_pStatGrid->setSizePolicy(sizePolicy4);
	m_pStatGrid->setMinimumSize(QSize(60, 0));
	m_pStatGrid->setMaximumSize(QSize(60, 16777215));

	layout->addWidget(m_pStatGrid);

	this->addPermanentWidget(m_pStatusWidget);

	m_pOptionsBtn->setText("Options");
	m_pMacroBtn->setText("Macro");
	m_pLogBtn->setText("Log");
	m_pStatBtn->setText("Stat");
	m_pBreakBtn->setText("Break");
	m_pStatusLbl->setText("Status:");
	m_pStatSelected->setText("Sel: 0");
	m_pStatCoords->setText("C: 0.0, 3.0, -10.0");
	m_pStatScreen->setText("Cur: 666, 1430");
	m_pStatGrid->setText("Grid: 1.0");
}

StatusBar::~StatusBar()
{
}
