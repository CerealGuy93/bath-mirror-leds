#include <U8g2lib.h>
#include <vector>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

class MenuEntry {
    public:

    void(*callback)();
    const char *text;

    MenuEntry(const char *text, void(*callback)()) {
        this->text = text;
        this->callback = callback;
    }
};

class Renderer {
    public:
    Renderer(U8G2 *display, std::vector<MenuEntry> entries) {
        this->display = display;
        this->entries = entries;

        this->currentMenuMax = entries.size();
    }

    void setup() {
        this->display->begin();

        this->render();
    }

   void render() {
        display->clearBuffer();

        for (auto i = 0; i < entries.size(); i++)
        {
            auto item = entries[i];
            renderEntry(i, item.text, this->menuState == i);
        }
        
        display->sendBuffer();	
    }

    void nextEntry() {
        this->menuState++;

        if (this->menuState >= this->currentMenuMax)
            this->menuState = 0;

        this->render();
    }

    void select() {
        auto target = entries[menuState];
        target.callback();
    }

    private:
    U8G2 *display;
    std::vector<MenuEntry> entries;
    int menuState = 0;
    int currentMenuMax = 0;


    void renderEntry(int index, const char *text, bool isActive) {
        auto y = 0;
        if (index == 0) {
            y = 10;
        } else {
            y = index * 10 + 10;
        }

        display->setFont(u8g2_font_6x10_mf);
        display->drawUTF8(15, y, text);

        display->setFont(u8g2_font_unifont_t_symbols);

        if (isActive) {
            display->drawGlyph(0, y + 1, 0x25cf);
        } else {
            display->drawGlyph(0, y + 1, 0x25cb);
        }
    }
};