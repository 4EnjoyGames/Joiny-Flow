#ifndef BROWSER_H
#define BROWSER_H
class Browser
{
public:
    enum URLId
    {
        FacebookGroup = 100,
        Site4Enjoy = 200,
        sendEmail = 300
    };

    static void openURL(const URLId id);
};

#endif // BROWSER_H
