# include <Siv3D.hpp> // OpenSiv3D v0.6.11

class GameManager {
private:
	std::vector<std::function<void()>> logicFunctions;   // List of functions for updating game logic
	std::vector<std::function<void()>> renderFunctions;  // List of functions for rendering graphics

public:
	GameManager()
	{
		Print << U"GameManagerConstructor logicFunctions.size():" << logicFunctions.size() << U" renderFunctions.size():" << renderFunctions.size();
	}
	int registerLogic(const std::function<void()>& func);
	int registerRender(const std::function<void()>& func);
	void unregisterLogic(int id);
	void unregisterRender(int id);
	void updateLogic();
	void updateRender();

};
// コールバックの登録時にユニークなIDを返す
int GameManager::registerLogic(const std::function<void()>& func) {
	logicFunctions.emplace_back(func);
	Print << U"RegisterLogic logicFunctions.size():" << logicFunctions.size();
	return logicFunctions.size() - 1; // IDはインデックスとする
}

int GameManager::registerRender(const std::function<void()>& func) {
	renderFunctions.emplace_back(func);
	Print << U"RegisterRender renderFunctions.size():" << renderFunctions.size();
	return renderFunctions.size() - 1; // IDはインデックスとする
}

// IDを使用して、特定のコールバックを削除する
void GameManager::unregisterLogic(int id) {
	// logicFunctions[id] = nullptr; // 関数を無効にする
	logicFunctions.erase(logicFunctions.begin() + id);
}

void GameManager::unregisterRender(int id) {
	// renderFunctions[id] = nullptr; // 関数を無効にする
	renderFunctions.erase(renderFunctions.begin() + id);

}
void GameManager::updateLogic() {
	for (const auto& func : logicFunctions) {
		if (func) {
			func();
		}
	}
}
void GameManager::updateRender() {
	for (const auto& func : renderFunctions) {
		if (func) {
			func();
		}
	}
}

class Clock
{
private:
	GameManager& gm_;
	const Font& font_;
	int logicId_;
	int renderId_;
	double time_;
	int height_;
public:
	Clock(GameManager& gm, const Font& font, int height) :gm_(gm), font_(font), time_(Scene::Time()), height_(height)
	{
		// 関数の登録時に返されるIDを保存する
		logicId_ = gm_.registerLogic([&]() { this->UpdateLogic(); });
		renderId_ = gm_.registerRender([&]() { this->UpdateRender(); });
		Print << U"Clock Register logicId_:" << logicId_ << U" renderId_:" << renderId_;
	}

	~Clock()
	{
		Print << U"Destruct Clock";
		gm_.unregisterLogic(logicId_);
		gm_.unregisterRender(renderId_);
		Print << U"Clock UnRegister logicId_:" << logicId_ << U" renderId_:" << renderId_;

	}

	void UpdateLogic()
	{
		time_ += Scene::DeltaTime();
	}
	void UpdateRender()
	{
		font_(U"{:.1f}"_fmt(time_)).draw();
	}

};

class ClockGenerator
{
private:
	GameManager& gm_;
	const Font& font_;
	int logicId_;
	int renderId_;
	double time_;
	int height_;
public:
	ClockGenerator(GameManager& gm, const Font& font, int height) :gm_(gm), font_(font), time_(Scene::Time()), height_(height)
	{
		// 関数の登録時に返されるIDを保存する
		logicId_ = gm_.registerLogic([&]() { this->UpdateLogic(); });
		renderId_ = gm_.registerRender([&]() { this->UpdateRender(); });
		Print << U"ClockGenerator Register logicId_:" << logicId_ << U" renderId_:" << renderId_;

	}
	~ClockGenerator()
	{
		Print << U"Destruct ClockGenerator";
	}
	void UpdateLogic()
	{
		if (KeySpace.down())
		{
			Print << U"Space down";
			std::unique_ptr<Clock> clock_ = std::make_unique<Clock>(gm_, font_, 0);
		}
	}
	void UpdateRender()
	{

	}
};


void Main()
{
	const Font font{ FontMethod::MSDF, 48, Typeface::Bold };
	GameManager gm;
	ClockGenerator clockGenerator(gm, font, 0);


	while (System::Update())
	{
		// clock.UpdateLogic();
		// clock.UpdateRender();

		// clockGenerator.UpdateLogic();
		// clockGenerator.UpdateRender();

		gm.updateLogic();
		gm.updateRender();
	}
}

