#include <iostream>
#include <string>

using namespace std;

class FileName
{
private:
    string name;

public:

    FileName(const string& name)
        : name(name)
    {}

    string getName() const
    {
        return name;
    }

    string getExtension() const
    {
        size_t pos = name.rfind('.');

        if (pos == string::npos)
            return "";

        return name.substr(pos + 1);
    }

    string getBaseName() const
    {
        size_t pos = name.rfind('.');

        if (pos == string::npos)
            return name;

        return name.substr(0, pos);
    }

    void changeExtension(const string& newExtension)
    {
        name = getBaseName() + "." + newExtension;
    }

    bool isImage() const
    {
        string ext = getExtension();

        return ext == "png" ||
            ext == "jpg" ||
            ext == "gif";
    }

    bool operator<(const FileName& other) const
    {
        return name < other.name;
    }

    void print() const
    {
        cout << name
            << " | ім'я: " << getBaseName()
            << " | розширення: [" << getExtension() << "]";

        if (isImage())
            cout << " | зображення";

        cout << endl;
    }
};


class MessageTemplate
{
private:
    string text;

    static void replaceAll(string& text,
        const string& from,
        const string& to)
    {
        size_t position = text.find(from);

        while (position != string::npos)
        {
            text.replace(position, from.length(), to);

            position = text.find(
                from,
                position + to.length()
            );
        }
    }

public:

    MessageTemplate(const string& text)
        : text(text)
    {}

    int countPlaceholders() const
    {
        int count = 0;

        for (int i = 0; i < text.length(); i++)
        {
            if (text[i] == '{')
                count++;
        }

        return count;
    }

    string fill(const string& name, int score) const
    {
        string result = text;

        replaceAll(result, "{name}", name);
        replaceAll(result, "{score}", to_string(score));

        return result;
    }
};


int main()
{
    FileName report("report.final.docx");
    FileName photo("photo.png");
    FileName readme("README");

    report.print();
    photo.print();
    readme.print();

    cout << endl;

    report.changeExtension("pdf");
    readme.changeExtension("md");

    report.print();
    readme.print();

    cout << endl;

    FileName copy = report;

    copy.changeExtension("txt");

    cout << "report: " << report.getName() << endl;
    cout << "copy: " << copy.getName() << endl;

    cout << endl;

    if (photo < report)
        cout << photo.getName() << " стоїть раніше" << endl;
    else
        cout << report.getName() << " стоїть раніше" << endl;


    MessageTemplate message(
        "Вітаю, {name}! Ваш бал за модуль: {score}. "
        "{name}, так тримати!"
    );

    string names[3] =
    {
        "Олена",
        "Марко",
        "Тарас"
    };

    int scores[3] =
    {
        11,
        9,
        12
    };

    cout << endl;

    cout << "Кількість місць: "
        << message.countPlaceholders()
        << endl;

    for (int i = 0; i < 3; i++)
    {
        cout << message.fill(names[i], scores[i]) << endl;
    }

    return 0;
}


/*
ПОЯСНИ

1. З char* треба було б знайти останню крапку вручну,
   виділити пам'ять і скопіювати частину рядка.
   string сам керує пам'яттю і дає rfind та substr.

2. Для "README" rfind поверне string::npos.
   Тому getExtension повертає "".

3. string сам керує своєю пам'яттю і правильно копіюється.
   Тому FileName не потребує власного деструктора і copy constructor.

4. "Вітаю, " і "Олена" — це const char*, їх не можна просто скласти через +.
   Якщо name — string, працює перевантажений operator+ для string.

5. Шукаємо далі після вставленого тексту, щоб не обробляти його повторно.
   Інакше можна отримати нескінченну заміну.

*/