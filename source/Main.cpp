#include <QApplication>
#include "LevelEditor.hpp"


int main(int argc, char* argv[])
{
	auto application{ std::make_unique<QApplication>(argc, argv) };

	QIcon icon;
	icon.addFile(QString::fromUtf8(":icons/LevelEditor_Icon.ico"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
	application->setWindowIcon(icon);

	auto frame{ std::make_unique<LevelEditorFrame>() };
	frame->show();

	return application->exec();
}
