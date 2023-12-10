#include "LevelEditor.hpp"
#include "ui_LevelEditor.h"

#include <utility>

#include <QMainWindow>
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>
#include <QRegExpValidator>
#include <QFile>
#include <QTextStream>
#include <QStyle>
#include <QAction>
#include <QSplashScreen>
#include <QTimer>
#include <QMenu>
#include <QMouseEvent>
#include <QRadioButton>

////////////////////////////////////////////////////////////////
// Some defines
////////////////////////////////////////////////////////////////

static QString dummy;
static QIcon dummy_icon;

static constexpr int POPUP_MENU_APPEAR_HEIGHT{ 10 };

static constexpr auto POPUP_MENU_SHEET{ R"(
	QMenu {
		border: 1px solid rgb(76, 76, 76);
	})"
};

////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////

QString fixPath(const QString& path) noexcept
{
	QString new_path;

	for (auto iter = path.begin(); iter != path.end(); ++iter)
	{
		if (const auto character = (*iter); character == '\\')
		{
			new_path += '/';

			auto next = std::next(iter);

			if (next != path.end() && (*next) == '\\')
				++iter;
		}
		else
		{
			new_path += character;
		}
	}

	return new_path;
}

LevelEditorFrame::LevelEditorFrame() :
	_ui{ std::make_unique<Ui_LevelEditor>() },
	m_DotIcon{ QIcon(":/icons/dot.png") }
{
	this->createSplash();

	_ui->setupUi(this);

	this->setupUI();

	this->connectActions();

	this->setFocus();

	//////////////////////////////////////////

	_ui->log_widget->close();

	//////////////////////////////////////////
}

LevelEditorFrame::~LevelEditorFrame()
{
	delete m_pSplash;
	m_pSplash = nullptr;
}

void LevelEditorFrame::createSplash()
{
	m_pSplash = new QSplashScreen(QPixmap(":/icons/Editor_Level_Splash.png"), Qt::WindowStaysOnTopHint);
	m_pSplash->show();

	this->setSplashText("Core Initializing...");

	QTimer::singleShot(2000, [this]()
	{
		this->setSplashText("Loading...");
	});

	QTimer::singleShot(4000, [this]()
	{
		this->finishLoading();
	});
}

void LevelEditorFrame::setSplashText(const QString& text)
{
	m_pSplash->showMessage(text, Qt::AlignCenter | Qt::AlignBottom, Qt::white);
}

void LevelEditorFrame::finishLoading()
{
	m_pSplash->close();
	//QTimer::singleShot(5000, m_pSplash, &QWidget::close);
}

void LevelEditorFrame::setupUI()
{
	_ui->toolbar->setFloating(false);

	this->switchToolbarGeom();

	//_ui->collapsed->setContent(_ui->content);
}

void LevelEditorFrame::connectActions()
{
	this->connect(_ui->dock_float_btn, &QToolButton::clicked, [this](bool checked)
	{
		_ui->toolbar->setFloating(!_ui->toolbar->isFloating());

		this->switchToolbarGeom();
	});

	this->connectCollapseToolWidget(_ui->scene_tool_collapse, _ui->scene_tool_widget);
	this->connectCollapseToolWidget(_ui->tools_tool_collapse, _ui->tools_tool_widget);
	this->connectCollapseToolWidget(_ui->edit_tool_collapse, _ui->edit_tool_widget);
	this->connectCollapseToolWidget(_ui->snap_tool_collapse, _ui->snap_tool_widget);
	this->connectCollapseToolWidget(_ui->commands_tool_collapse, _ui->commands_tool_widget);
	this->connectCollapseToolWidget(_ui->ref_select_tool_collapse, _ui->ref_select_tool_widget);
	this->connectCollapseToolWidget(_ui->current_object_tool_collapse, _ui->current_object_tool_widget);

	this->connect(_ui->collapse_all_btn, &QToolButton::clicked, [this](bool checked)
	{
		_ui->scene_tool_widget->setVisible(false);
		_ui->tools_tool_widget->setVisible(false);
		_ui->edit_tool_widget->setVisible(false);
		_ui->snap_tool_widget->setVisible(false);
		_ui->commands_tool_widget->setVisible(false);
		_ui->ref_select_tool_widget->setVisible(false);
		_ui->current_object_tool_widget->setVisible(false);
	});

	this->connect(_ui->expand_all_btn, &QToolButton::clicked, [this](bool checked)
	{
		_ui->scene_tool_widget->setVisible(true);
		_ui->tools_tool_widget->setVisible(true);
		_ui->edit_tool_widget->setVisible(true);
		_ui->snap_tool_widget->setVisible(true);
		_ui->commands_tool_widget->setVisible(true);
		_ui->ref_select_tool_widget->setVisible(true);
		_ui->current_object_tool_widget->setVisible(true);
	});

	this->connect(_ui->statusbar->getLogBtn(), &QToolButton::clicked, [this](bool)
	{
		if (_ui->log_widget->isVisible())
			_ui->log_widget->close();
		else
			_ui->log_widget->show();
	});

	this->connect(_ui->statusbar->getStatBtn(), &QToolButton::clicked, [this](bool)
	{
		// Show/hide stats
	});

	this->connect(_ui->statusbar->getBreakBtn(), &QToolButton::clicked, [this](bool)
	{
		// Breaks action
	});

	this->connect(_ui->log_reset_size_btn, &QPushButton::clicked, [this](bool)
	{
		if (_ui->log_widget->isFloating())
			_ui->log_widget->resize(_ui->log_widget->minimumSize());
	});

	////////////////////////////////////////////////////////////////////////////////
	// Menus connections
	////////////////////////////////////////////////////////////////////////////////

	// Whole menu creation structure
	const auto& scene_file_submenus{ this->makePopupMenu({ _ui->menu_file_btn }, {
		{ "Clear", nullptr },
		{ "Open", nullptr },
		{ "Save", nullptr },
		{ "Save As...", nullptr },
		{ {}, nullptr },
		{ "Open Selection...", nullptr },
		{ "Save Selection As...", nullptr },
		{ {}, nullptr },
		{ "Make Pack...", nullptr },
		{ {}, nullptr },
		{ ">Open Recent", nullptr },
		{ {}, nullptr },
		{ "Quit", &LevelEditorFrame::closeApp }
	}) };

	this->makePopupMenu({ _ui->menu_scene_btn }, {
		{ "Options", nullptr },
		{ {}, nullptr },
		{ "Validate", nullptr },
		{ {}, nullptr },
		{ "Summary Info", nullptr },
		{ "Highlight Texture...", nullptr },
		{ {}, nullptr },
		{ "Clear Debug Draw", nullptr },
		{ {}, nullptr },
		{ "Export entire Scene as Obj", nullptr },
		{ "Export selection as Obj", nullptr }
	});

	this->makePopupMenu({ _ui->menu_compile_btn }, {
		{ "Build", nullptr },
		{ {}, nullptr },
		{ "Make Game", nullptr },
		{ "Make Details", nullptr },
		{ "Make HOM", nullptr },
		{ "Make Sound Occluder", nullptr },
		{ "Make AI-Map", nullptr },
		{ {}, nullptr },
		{ "Import Error List", nullptr },
		{ "Export Error List", nullptr },
		{ "Clear Error List", nullptr }
	});

	this->makePopupMenu({ _ui->menu_objects_btn }, {
		{ "Library Editor", nullptr },
		{ {}, nullptr },
		{ "Reload Objects", nullptr },
		{ "Clean Library", nullptr },
		{ "ClipEditor", nullptr }
	});

	this->makePopupMenu({ _ui->menu_images_btn }, {
		{ "Image Editor", nullptr },
		{ {}, nullptr },
		{ "Reload Textures", nullptr },
		{ {}, nullptr },
		{ "Synchronize Textures", nullptr },
		{ "Check New Textures", nullptr },
		{ {}, nullptr },
		{ "Edit minimap", nullptr },
		{ "SyncTHM", nullptr }
	});

	this->makePopupMenu({ _ui->menu_sounds_btn }, {
		{ "Sound Editor", nullptr },
		{ {}, nullptr },
		{ "Synchronize Sounds", nullptr },
		{ {}, nullptr },
		{ "Refresh Environment Library", nullptr },
		{ "Refresh Environment Geometry", nullptr }
	});

	this->makePopupMenu({ _ui->menu_edit_btn }, {
		{ "Cut", nullptr },
		{ "Copy", nullptr },
		{ "Paste", nullptr },
		{ "Delete", nullptr },
		{ {}, nullptr },
		{ "Undo", nullptr },
		{ "Redo", nullptr }
	});

	this->makePopupMenu({ _ui->menu_visibility_btn }, {
		{ "Hide Selected", nullptr },
		{ "Hide Unselected", nullptr },
		{ "Hide All", nullptr },
		{ {}, nullptr },
		{ "Unhide All", nullptr }
	});

	this->makePopupMenu({ _ui->menu_selection_btn }, {
		{ "Invert", nullptr },
		{ "Select All", nullptr },
		{ "Unselect All", nullptr }
	});

	this->makePopupMenu({ _ui->menu_commands_btn }, {
		{ "Make List From Selected", nullptr },
		{ "Select Object From List", nullptr },
		{ {}, nullptr },
		{ "Add Selected To List", nullptr },
		{ "Remove Selected From List", nullptr },
		{ {}, nullptr },
		{ "Clear List", nullptr }
	});

	///////////////////////////////////////////////////////////
	// Statusbar items
	///////////////////////////////////////////////////////////

	// Whole menu creation structure
	const auto& statusbar_opt_submenus{ this->makePopupMenu({ _ui->statusbar->getOptionsBtn(), _ui->statusbar->getOptionsGrip() }, {
		{ ">Render", nullptr },
		{ {}, nullptr },
		{ "Draw Safe Rect", nullptr, MenuItem::Type::CHECKBOX },
		{ "Draw Grid", nullptr, MenuItem::Type::CHECKBOX, true },
		{ {}, nullptr },
		{ ">Weather", nullptr },
		{ "Fog", nullptr, MenuItem::Type::CHECKBOX },
		{ {}, nullptr },
		{ "Mute Sounds", nullptr, MenuItem::Type::CHECKBOX },
		{ "Light Scene", nullptr, MenuItem::Type::CHECKBOX },
		{ "Real Time", nullptr, MenuItem::Type::CHECKBOX, true },
		{ ">Device", nullptr }
	}) };

	const auto& statusbar_macro_submenus{ this->makePopupMenu({ _ui->statusbar->getMacroBtn(), _ui->statusbar->getMacroGrip() }, {
		{ ">1: <empty>", nullptr },
		{ ">2: <empty>", nullptr },
		{ ">3: <empty>", nullptr },
		{ ">4: <empty>", nullptr },
		{ ">5: <empty>", nullptr },
		{ ">6: <empty>", nullptr },
		{ ">7: <empty>", nullptr },
		{ ">8: <empty>", nullptr },
		{ {}, nullptr },
		{ "Edit Commands List...", nullptr },
		{ "Log Commands", nullptr, MenuItem::Type::CHECKBOX, false }
	}) };

	for (auto macro_menu : statusbar_macro_submenus)
	{
		const auto& render_menus{ this->makePopupMenu({}, {
			{ "Execute", nullptr, MenuItem::Type::BUTTON, false, false },
			{ "Assign", nullptr },
			{ "Clear", nullptr }
		}, macro_menu) };
	}

	auto pRecentListMenu{ scene_file_submenus.front() };

	auto iter{ statusbar_opt_submenus.cbegin() };
	{
		const auto& render_menus{ this->makePopupMenu({}, {
			{ ">Quality", nullptr },
			{ ">Fill Mode", nullptr },
			{ ">Shade Mode", nullptr },
			{ "Edged Faces", nullptr, MenuItem::Type::CHECKBOX },
			{ {}, nullptr },
			{ "HW Render", nullptr, MenuItem::Type::CHECKBOX, true },
			{ {}, nullptr },
			{ "Linear Filter", nullptr, MenuItem::Type::CHECKBOX, true },
			{ "Textures", nullptr, MenuItem::Type::CHECKBOX, true },
		}, (*iter)) };

		++iter;

		this->makePopupMenu({}, {
			{ "none", nullptr, MenuItem::Type::RADIO, true },
			{ {}, nullptr },
			{ "Reload", nullptr },
			{ {}, nullptr },
			{ "Properties...", nullptr }
		}, (*iter));

		++iter;

		const auto& time_factor_menu{ this->makePopupMenu({}, {
			{ ">Time Factor", nullptr }
		}, (*iter)) };

		this->makePopupMenu({}, {
			{ "25%", nullptr, MenuItem::Type::RADIO },
			{ "50%", nullptr, MenuItem::Type::RADIO },
			{ "75%", nullptr, MenuItem::Type::RADIO },
			{ "100%", nullptr, MenuItem::Type::RADIO },
			{ "125%", nullptr, MenuItem::Type::RADIO },
			{ "150%", nullptr, MenuItem::Type::RADIO },
			{ "200%", nullptr, MenuItem::Type::RADIO, true }
		}, render_menus[0]);

		this->makePopupMenu({}, {
			{ "Point", nullptr, MenuItem::Type::RADIO },
			{ "Wireframe", nullptr, MenuItem::Type::RADIO },
			{ "Solid", nullptr, MenuItem::Type::RADIO, true }
		}, render_menus[1]);

		this->makePopupMenu({}, {
			{ "Flat", nullptr, MenuItem::Type::RADIO },
			{ "Gourand", nullptr, MenuItem::Type::RADIO, true }
		}, render_menus[2]);

		this->makePopupMenu({}, {
			{ "0.05", nullptr, MenuItem::Type::RADIO },
			{ "0.1", nullptr, MenuItem::Type::RADIO },
			{ "0.2", nullptr, MenuItem::Type::RADIO },
			{ "0.3", nullptr, MenuItem::Type::RADIO },
			{ "0.5", nullptr, MenuItem::Type::RADIO },
			{ "1.0", nullptr, MenuItem::Type::RADIO, true },
			{ "3.0", nullptr, MenuItem::Type::RADIO },
			{ "5.0", nullptr, MenuItem::Type::RADIO },
			{ "10.0", nullptr, MenuItem::Type::RADIO }
		}, time_factor_menu.first());
	}
}

// empty string - separator
QList<QMenu*> LevelEditorFrame::makePopupMenu(QList<QToolButton*>&& buttons_to_connect, QList<MenuItem>&& items_list, QMenu* parent)
{
	QMenu* menu{ (parent != nullptr) ? parent : this->createPopupMenu() };

	menu->setStyleSheet(POPUP_MENU_SHEET);

	QList<QAction*> actions;
	QList<QMenu*> menus;

	{
		menu->clear();

		const bool has_radio_btn = std::any_of(items_list.cbegin(), items_list.cend(), [](const MenuItem& item) -> bool { return (item.type == MenuItem::Type::RADIO); });

		QActionGroup* group{ nullptr };

		if (has_radio_btn)
		{
			group = new QActionGroup(menu);
			group->setExclusive(true);

			QObject::connect(group, &QActionGroup::triggered, [this](QAction* action)
			{
				auto type{ static_cast<MenuItem::Type>(action->data().toInt()) };

				action->setIcon(((type == MenuItem::Type::RADIO) && action->isChecked()) ? m_DotIcon : dummy_icon);
				//action->setIconVisibleInMenu(((type == MenuItem::Type::RADIO) && action->isChecked()));
			});
		}

		for (auto&& item : items_list)
		{
			if (item.item_name.isEmpty()) // separator
			{
				menu->addSeparator();
			}
			else if (item.item_name[0] == '>') // submenu
			{
				auto submenu{ menu->addMenu(item.item_name.mid(1)) };
				menus.push_back(submenu);
			}
			else
			{
				auto action{ new QAction(item.item_name, this) };
				action->setData(QVariant(static_cast<int>(item.type)));
				action->setEnabled(item.enabled);

				switch (item.type)
				{
					case MenuItem::Type::BUTTON:
					break;

					case MenuItem::Type::CHECKBOX:
					{
						action->setCheckable(true);
						action->setChecked(item.checked);
					}
					break;

					case MenuItem::Type::RADIO:
					{
						if (group != nullptr)
						{
							group->addAction(action);

							action->setCheckable(true);
							action->setChecked(item.checked);
						}
					}
					break;

					default:
					break;
				}

				if (item.func_ptr != nullptr)
					this->connect(action, &QAction::triggered, this, item.func_ptr);

				menu->addAction(action);

				actions.push_back(action);
			}
		}
	}

	for (auto&& button : buttons_to_connect)
	{
		this->connect(button, &QToolButton::clicked, [this, menu](bool checked)
		{
			const auto& pos{ this->getMousePosition() };
			menu->move(pos - QPoint{ menu->width() / 2, POPUP_MENU_APPEAR_HEIGHT });

			menu->exec();
		});
	}

	return menus;
}

void LevelEditorFrame::log(const char* message)
{
	//_ui->log->appendPlainText(QString::fromUtf8(message));
	//_ui->log->update();
}

void LevelEditorFrame::log(const QString& message)
{
	//_ui->log->appendPlainText(message);
	//_ui->log->update();
}

void LevelEditorFrame::msg(const QString& message)
{
	QMessageBox dlg;
	dlg.setText(message);
	dlg.exec();
}

QPoint LevelEditorFrame::getMousePosition() const noexcept
{
	return QCursor::pos();
}

void LevelEditorFrame::switchToolbarGeom()
{
	if (auto pTitle{ _ui->toolbar->titleBarWidget() }; pTitle != nullptr)
	{
		delete pTitle;

		_ui->toolbar->setTitleBarWidget(nullptr);
	}

	if (_ui->toolbar->isFloating())
	{
		auto pDummyWidget{ new QWidget(_ui->centralwidget) };

		pDummyWidget->setStyleSheet("QWidget { border: 1px solid rgb(232, 232, 232); }");

		QSizePolicy policy(QSizePolicy::Preferred, QSizePolicy::Fixed);
		policy.setHorizontalStretch(0);
		policy.setVerticalStretch(0);
		policy.setHeightForWidth(false);

		pDummyWidget->setSizePolicy(policy);

		auto pLayout{ new QHBoxLayout() };
		pDummyWidget->setLayout(pLayout);

		_ui->toolbar->setTitleBarWidget(pDummyWidget);

		_ui->toolbar->move(this->pos() + QPoint(width() / 2, 0) - QPoint(_ui->toolbar->width() / 2, 0));
	}
	else
	{
		auto pDummyWidget{ new QWidget(_ui->centralwidget) };

		_ui->toolbar->setTitleBarWidget(pDummyWidget);
	}
}

void LevelEditorFrame::connectCollapseToolWidget(QToolButton* pButton, QWidget* pToolWidget)
{
	this->connect(pButton, &QToolButton::clicked, [pButton, pToolWidget](bool checked)
	{
		pToolWidget->setVisible(!pToolWidget->isVisible());
	});
}

void LevelEditorFrame::closeApp(bool)
{
	this->close();
}
