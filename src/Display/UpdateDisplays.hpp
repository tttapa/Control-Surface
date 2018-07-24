void updateDisplays() {
#ifdef DISPLAY_GFX
    static unsigned long previousRefresh = millis();

    if (millis() - previousRefresh < 1000 / MAX_FPS)
        return;
    previousRefresh += 1000 / MAX_FPS;
    DEBUGFN("GOOD");

    for (DisplayInterface *display = DisplayInterface::getFirst();
         display != nullptr; display = display->getNext())
        display->clearDisplay();

    for (DisplayElement *de = DisplayElement::getFirst(); de != nullptr;
         de = de->getNext())
        de->draw();

    for (DisplayInterface *display = DisplayInterface::getFirst();
         display != nullptr; display = display->getNext())
        display->display();
#endif
}