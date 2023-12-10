#include "CollapseWidget.hpp"
#include <QApplication>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QToolButton>
#include <QVBoxLayout>
#include <QLabel>

static constexpr int ARROW_SIZE{ 8 };

CollapseWidget::CollapseWidget(QWidget* parent) : QWidget(parent),
	m_pLayout{ new QVBoxLayout(this) },
	m_pAnimator{ new QParallelAnimationGroup(this) },
	m_pCap{ new QWidget(this) }
{
	m_pLayout->setContentsMargins(0, 0, 0, 0);

	this->setStyleSheet("border: 1px solid rgb(200, 200, 200);");

	QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
	sizePolicy1.setHorizontalStretch(0);
	sizePolicy1.setVerticalStretch(0);

	sizePolicy1.setHeightForWidth(m_pCap->sizePolicy().hasHeightForWidth());
	m_pCap->setSizePolicy(sizePolicy1);
	m_pCap->setStyleSheet("background-color: rgb(135, 135, 135);");
	m_pCapLayout = new QHBoxLayout(m_pCap);
	m_pCapLayout->setSpacing(2);

	m_pCapLayout->setContentsMargins(1, 1, 1, 1);
	pSpacerLeft = new QSpacerItem(147, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

	m_pCapLayout->addItem(pSpacerLeft);

	m_pText = new QLabel(m_pCap);
	m_pText->setStyleSheet("border: none;");
	m_pText->setText("description");

	m_pCapLayout->addWidget(m_pText);

	pSpacerRight = new QSpacerItem(146, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

	m_pCapLayout->addItem(pSpacerRight);

	m_pCollapseButton = new QToolButton(m_pCap);
	m_pCollapseButton->setMinimumSize(QSize(ARROW_SIZE * 2, ARROW_SIZE * 2));
	m_pCollapseButton->setMaximumSize(QSize(ARROW_SIZE * 2, ARROW_SIZE * 2));

	m_pCapLayout->addWidget(m_pCollapseButton);

	m_pLayout->addWidget(m_pCap);

	m_pCollapseButton->setCheckable(true);
	m_pCollapseButton->setStyleSheet("background: none; border: none;");
	m_pCollapseButton->setIconSize(QSize(ARROW_SIZE, ARROW_SIZE));
	m_pCollapseButton->setFont(QApplication::font());
	m_pCollapseButton->setArrowType(Qt::ArrowType::LeftArrow);

	this->connect(m_pCollapseButton, &QToolButton::toggled, [=](bool checked)
	{
		m_pCollapseButton->setArrowType(checked ? Qt::ArrowType::DownArrow : Qt::ArrowType::LeftArrow);
		((m_pContent != nullptr) && checked) ? this->showContent() : this->hideContent();
	});
}

CollapseWidget::~CollapseWidget()
{
	delete m_pAnimator;
	m_pAnimator = nullptr;
}

void CollapseWidget::setText(const QString& text)
{
	m_pText->setText(text);
}

void CollapseWidget::setContent(QWidget* content)
{
	assert(content != nullptr);

	m_pContent = content;

	auto pAnimation{ new QPropertyAnimation(m_pContent, "maximumHeight") };
	pAnimation->setStartValue(0);
	pAnimation->setEasingCurve(QEasingCurve::InOutQuad);
	pAnimation->setDuration(300);
	pAnimation->setEndValue(m_pContent->geometry().height() + 10);

	m_pAnimator->addAnimation(pAnimation);

	if (!m_pCollapseButton->isChecked())
		m_pContent->setMaximumHeight(0);
}

void CollapseWidget::hideContent()
{
	m_pAnimator->setDirection(QAbstractAnimation::Backward);
	m_pAnimator->start();
}

void CollapseWidget::showContent()
{
	m_pAnimator->setDirection(QAbstractAnimation::Forward);
	m_pAnimator->start();
}
