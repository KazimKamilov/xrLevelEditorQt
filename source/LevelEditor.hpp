#ifndef _LEVEL_EDITOR_HPP_
#define _LEVEL_EDITOR_HPP_

#include <memory>
#include <map>
#include <vector>
#include <functional>
#include <QMainWindow>
#include <QIcon>


class Ui_LevelEditor;
class QSettings;
class QToolButton;
class QSplashScreen;


class LevelEditorFrame final : public QMainWindow
{
	Q_OBJECT

	std::unique_ptr<Ui_LevelEditor> _ui;
	QSplashScreen* m_pSplash{ nullptr };
	QIcon m_DotIcon;

public:
	LevelEditorFrame();
	~LevelEditorFrame() override;

	void log(const QString& message);
	void log(const char* message);
	void msg(const QString& message);

	QPoint getMousePosition() const noexcept;

protected:
	struct MenuItem final
	{
		enum class Type : uint8_t
		{
			BUTTON,
			CHECKBOX,
			RADIO
		};

		QString item_name;
		void (LevelEditorFrame::* func_ptr)(bool){ nullptr };
		Type type{ Type::BUTTON };
		bool checked{ false };
		bool enabled{ true };
	};

private:
	void setupUI();
	void connectActions();

	void createSplash();
	void setSplashText(const QString& text);
	void finishLoading();

	QList<QMenu*> makePopupMenu(QList<QToolButton*>&& buttons_to_connect, QList<MenuItem>&& items_list, QMenu* parent = nullptr);

	void switchToolbarGeom();
	void connectCollapseToolWidget(QToolButton* pButton, QWidget* pToolWidget);

	// slots
	void closeApp(bool);

};

#endif
