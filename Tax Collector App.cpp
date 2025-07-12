#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>
using namespace sf;
using namespace std;
// Payment struct to store payment details
struct Payment {
    string paymentDate;
    double amount;
    string taxType;
};
// Helper function to get the current date as a string
string getCurrentDate() {
    time_t now = time(0);
    tm localTime;
    localtime_s(&localTime, &now); // Use localtime_s instead of localtime
    stringstream dateStream;
    dateStream << setw(2) << setfill('0') << localTime.tm_mon + 1 << "/"
        << setw(2) << setfill('0') << localTime.tm_mday << "/"
        << localTime.tm_year + 1900;
    return dateStream.str();
}
// Main Tax Collector App class
class TaxCollectorApp {
private:
    vector<Payment> paymentHistory;
public:
    double calculateTax(string taxType, double amount) {
        double taxRate = 0.0;
        if (taxType == "Income") {
            taxRate = 0.15; // 15% income tax
        }
        else if (taxType == "Property") {
            taxRate = 0.10; // 10% property tax
        }
        else {
            return -1.0; // Invalid tax type
        }
        return amount * taxRate;
    }
    void makePayment(string taxType, double amount) {
        double calculatedTax = calculateTax(taxType, amount);
        if (calculatedTax > 0) {
            Payment payment;
            payment.paymentDate = getCurrentDate();
            payment.amount = calculatedTax;
            payment.taxType = taxType;
            paymentHistory.push_back(payment);
        }
    }
    vector<Payment> getPaymentHistory() {
        return paymentHistory;
    }
};
// Function to create and run the app window
void runAppWindow(Font& font, TaxCollectorApp& app) {
    RenderWindow appWindow(VideoMode(800, 600), "Tax Collector App");
    // Text inputs and labels
    Text title("Tax Collector App", font, 40);
    title.setPosition(200, 20);
    title.setFillColor(Color::White);
    Text taxTypeLabel("Enter Tax Type (Income/Property):", font, 20);
    taxTypeLabel.setPosition(50, 100);
    taxTypeLabel.setFillColor(Color::White);
    Text amountLabel("Enter Amount:", font, 20);
    amountLabel.setPosition(50, 180);
    amountLabel.setFillColor(Color::White);
    Text historyLabel("Payment History:", font, 20);
    historyLabel.setPosition(50, 260);
    historyLabel.setFillColor(Color::White);
    RectangleShape inputBox(Vector2f(400, 30));
    inputBox.setPosition(50, 140);
    inputBox.setFillColor(Color::White);
    RectangleShape amountBox(Vector2f(400, 30));
    amountBox.setPosition(50, 220);
    amountBox.setFillColor(Color::White);
    RectangleShape payButton(Vector2f(150, 40));
    payButton.setPosition(500, 140);
    payButton.setFillColor(Color::Green);
    Text payButtonText("Pay", font, 20);
    payButtonText.setPosition(540, 145);
    payButtonText.setFillColor(Color::White);
    vector<Text> historyText;
    string taxTypeInput = "";
    string amountInput = "";
    bool typingTaxType = true;
    while (appWindow.isOpen()) {
        Event event;
        while (appWindow.pollEvent(event)) {
            if (event.type == Event::Closed)
                appWindow.close();
            if (event.type == Event::TextEntered) {
                if (typingTaxType) {
                    if (event.text.unicode == 8 && !taxTypeInput.empty()) { // Backspace
                        taxTypeInput.pop_back();
                    }
                    else if (event.text.unicode < 128) {
                        taxTypeInput += static_cast<char>(event.text.unicode);
                    }
                }
                else {
                    if (event.text.unicode == 8 && !amountInput.empty()) { // Backspace
                        amountInput.pop_back();
                    }
                    else if (event.text.unicode < 128) {
                        amountInput += static_cast<char>(event.text.unicode);
                    }
                }
            }
            if (event.type == Event::MouseButtonPressed) {
                Vector2f mousePos = appWindow.mapPixelToCoords(Mouse::getPosition(appWindow));
                if (inputBox.getGlobalBounds().contains(mousePos)) {
                    typingTaxType = true;
                }
                else if (amountBox.getGlobalBounds().contains(mousePos)) {
                    typingTaxType = false;
                }
                else if (payButton.getGlobalBounds().contains(mousePos)) {
                    try {
                        double amount = stod(amountInput);
                        app.makePayment(taxTypeInput, amount);
                        // Update history text
                        historyText.clear();
                        auto history = app.getPaymentHistory();
                        for (size_t i = 0; i < history.size(); ++i) {
                            stringstream ss;
                            ss << history[i].paymentDate << " | "
                                << history[i].taxType << " | $" << fixed << setprecision(2) << history[i].amount;
                            Text record(ss.str(), font, 16);
                            record.setPosition(50, 300 + i * 25);
                            record.setFillColor(Color::White);
                            historyText.push_back(record);
                        }
                        taxTypeInput = "";
                        amountInput = "";
                    }
                    catch (exception& e) {
                        cout << "Invalid input." << endl;
                    }
                }
            }
        }
        appWindow.clear(Color::Blue);
        // Draw UI elements
        appWindow.draw(title);
        appWindow.draw(taxTypeLabel);
        appWindow.draw(amountLabel);
        appWindow.draw(historyLabel);
        appWindow.draw(inputBox);
        appWindow.draw(amountBox);
        appWindow.draw(payButton);
        appWindow.draw(payButtonText);
        // Draw inputs
        Text taxTypeText(taxTypeInput, font, 20);
        taxTypeText.setPosition(55, 140);
        taxTypeText.setFillColor(Color::Black);
        appWindow.draw(taxTypeText);
        Text amountText(amountInput, font, 20);
        amountText.setPosition(55, 220);
        amountText.setFillColor(Color::Black);
        appWindow.draw(amountText);
        // Draw history
        for (const auto& record : historyText) {
            appWindow.draw(record);
        }
        appWindow.display();
    }
}
int main() {
    TaxCollectorApp app;
    RenderWindow w1(VideoMode(600.f, 600.f), "Tax Collector App");
    Font font;
    if (!font.loadFromFile("arial.ttf"))
        cout << "Can't load font";
    Text t("Welcome To", font, 45);
    t.setFillColor(Color::White);
    t.setPosition(Vector2f(160.f, 150.f));
    t.setOutlineThickness(5.f);
    t.setOutlineColor(Color::Black);
    Text t1("Tax Collector App", font, 45);
    t1.setFillColor(Color::White);
    t1.setPosition(Vector2f(120.f, 250.f));
    t1.setOutlineThickness(5.f);
    t1.setOutlineColor(Color::Black);
    RectangleShape box(Vector2f(105.f, 50.f));
    box.setFillColor(Color::Transparent);
    box.setPosition(Vector2f(250.f, 350.f));
    Text r("Start", font, 45);
    r.setFillColor(Color::Red);
    r.setPosition(Vector2f(255.f, 348.f));
    r.setOutlineThickness(5.f);
    r.setOutlineColor(Color::Black);
    while (w1.isOpen()) {
        Event event;
        while (w1.pollEvent(event)) {
            if (event.type == Event::Closed)
                w1.close();
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
                w1.close();
            // Handle resizing
            if (event.type == Event::Resized) {
                // Create a logical view (fixed coordinate system)
                View view(FloatRect(0, 0, 600, 600));  // Logical coordinate system: width=600, height=500
                w1.setView(view);
                // Adjust the view to match the new window size
                float aspectRatio = static_cast<float>(event.size.width) / event.size.height;
                view.setSize(600 * aspectRatio, 600);  // Adjust view size proportionally
                w1.setView(view);
            }
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2f mousePosition = w1.mapPixelToCoords(Mouse::getPosition(w1));
                if (box.getGlobalBounds().contains(mousePosition)) {
                    w1.close();
                    runAppWindow(font, app);
                }
            }
        }
        w1.clear(Color::Magenta);
        w1.draw(t);
        w1.draw(t1);
        w1.draw(box);
        w1.draw(r);
        w1.display();
    }
    return 0;
}
