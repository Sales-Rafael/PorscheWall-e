#include <SoftwareSerial.h>
#include <Servo.h>

Servo s;

SoftwareSerial bluetooth(10, 11);  // RX, TX

#define pinoServo 12
#define trigPin 9
#define echoPin 2
#define p2m2 8
#define p1m2 7
#define pvelm2 6
#define p2m1 5
#define p1m1 4
#define pvelm1 3
#define buzzerPin A0  // Buzzer pin set to A0 (digital pin 14)

int vel;
bool sensorAtivo = false;  // Tracks whether sensor-based movement is active

class Motor {
public:
    int p1, p2, pv;

    Motor(int p1, int p2, int v) {
        this->p1 = p1;
        this->p2 = p2;
        this->pv = v;
        pinMode(p1, OUTPUT);
        pinMode(p2, OUTPUT);
        pinMode(pv, OUTPUT);
        digitalWrite(p1, LOW);
        digitalWrite(p2, LOW);
        analogWrite(pv, 0);
    }

    void frente(int v) {
        digitalWrite(p1, HIGH);
        digitalWrite(p2, LOW);
        analogWrite(pv, v);
    }

    void tras(int v) {
        digitalWrite(p1, LOW);
        digitalWrite(p2, HIGH);
        analogWrite(pv, v);
    }

    void para() {
        digitalWrite(p1, LOW);
        digitalWrite(p2, LOW);
        analogWrite(pv, 0);
    }

    void freia() {
        digitalWrite(p1, HIGH);
        digitalWrite(p2, HIGH);
        analogWrite(pv, 255);
    }
};

Motor *md = new Motor(p1m1, p2m1, pvelm1);
Motor *me = new Motor(p1m2, p2m2, pvelm2);

void virarEsquerda(int v) {
    md->frente(v);
    me->tras(v);
}

void virarDireita(int v) {
    md->tras(v);
    me->frente(v);
}

void irFrente(int v) {
    md->frente(v);
    me->frente(v);
}

void irTras(int v) {
    md->tras(v);
    me->tras(v);
}

void parar() {
    md->para();
    me->para();
}

void freiar() {
    md->freia();
    me->freia();
}

void frenteLongo(){
    irFrente(vel);
    delay(1000);
    parar();
    delay(500);
}

void frenteCurto(){
    irFrente(vel);
    delay(500);
    parar();
    delay(500);
}

void direita90graus(){
    virarDireita(vel);
    delay(1100);
    parar();
    delay(500);
}

void direita360graus(){
    virarDireita(vel);
    delay(2250);
    parar();
    delay(500);
}

void esquerda90graus(){
    virarEsquerda(vel);
    delay(1100);  // 90-degree left turn
    parar();
    delay(500);
}

void esquerda360graus(){
    virarEsquerda(vel);
    delay(2250);  // 360-degree left turn
    parar();
    delay(500);
}

void direita45graus(){
    virarDireita(vel);
    delay(550);
    parar();
    delay(500);
}

void direita22graus(){
    virarDireita(vel);
    delay(125);
    parar();
    delay(500);
}

void esquerda45graus(){
    virarEsquerda(vel);
    delay(550);
    parar();
    delay(500);
}

void rotaSimples1(){
    frenteLongo();
    direita90graus();
    frenteCurto();
    direita90graus();
    frenteLongo();
    esquerda90graus();
    frenteCurto();
    esquerda90graus();
    frenteLongo();
    direita90graus();
    frenteCurto();
    direita90graus();
    frenteLongo();
    esquerda90graus();
    frenteCurto();
    esquerda90graus();
}

void rotaSimples2(){
    md->frente(vel*4);
    me->frente(vel);
    delay(8000);
    parar();
    delay(500);
}

void rotaSimples3(){
    direita45graus();
    frenteLongo();
    direita90graus();
    frenteLongo();
    esquerda90graus();
    frenteLongo();
    direita90graus();
    frenteLongo();
}

void rotaSimples4(){
    frenteLongo();
    direita360graus();
    frenteLongo();
    direita90graus();
    frenteLongo();
    direita360graus();
    frenteLongo();
    direita90graus();
    frenteLongo();
    direita360graus();
    frenteLongo();
    direita90graus();
    frenteLongo();
    direita360graus();
    frenteLongo();
    direita90graus();
}

void rotaSimples5(){
    frenteCurto();
    frenteCurto();
    frenteCurto();
}

// Function to measure distance using ultrasonic sensor
long medirDistancia() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    long duration = pulseIn(echoPin, HIGH);
    long distance = duration * 0.034 / 2;  // Convert to cm
    return distance;
}

// Function to sound the buzzer
void ativarBuzzer() {
    tone(buzzerPin, 1000);  // 1kHz tone
    delay(500);
    noTone(buzzerPin);
}

// Function to scan for clear path with five angles per side
void scanForClearPath() {
    long distanciasEsquerda[5], distanciasDireita[5], distanciaCentro;
    long maxDistancia = 0;  // Initialize to 0 for distance comparison
    int maxAngulo = 90;  // Default to center (forward)

    // Scan left side (0° to 60°, 15° increments)
    for (int i = 0; i < 5; i++) {
        s.write(15 * i);  // Angles: 0°, 15°, 30°, 45°, 60°
        delay(300);
        distanciasEsquerda[i] = medirDistancia();
        if (distanciasEsquerda[i] > maxDistancia) {
            maxDistancia = distanciasEsquerda[i];
            maxAngulo = 15 * i;
        }
    }

    // Scan center (90°)
    s.write(90);
    delay(300);
    distanciaCentro = medirDistancia();
    if (distanciaCentro > maxDistancia) {
        maxDistancia = distanciaCentro;
        maxAngulo = 90;
    }

    // Scan right side (120° to 180°, 15° increments)
    for (int i = 0; i < 5; i++) {
        s.write(120 + 15 * i);  // Angles: 120°, 135°, 150°, 165°, 180°
        delay(300);
        distanciasDireita[i] = medirDistancia();
        if (distanciasDireita[i] > maxDistancia) {
            maxDistancia = distanciasDireita[i];
            maxAngulo = 120 + 15 * i;
        }
    }

    // Return servo to center
    s.write(90);
    delay(300);

    // Decide direction based on maximum distance
    if (maxAngulo <= 60) {  // Left side (0° to 60°)
        virarEsquerda(vel);
        delay(1100);  // Turn left 90 degrees
        parar();
        delay(500);
        // Check forward direction after turning
        s.write(90);  // Ensure servo is facing forward
        delay(300);
        long novaDistancia = medirDistancia();
        if (novaDistancia < 30 && novaDistancia > 0) {
            scanForClearPath();  // Rescan if obstacle detected
        }
    } else if (maxAngulo >= 120) {  // Right side (120° to 180°)
        virarDireita(vel);
        delay(1100);  // Turn right 90 degrees
        parar();
        delay(500);
        // Check forward direction after turning
        s.write(90);  // Ensure servo is facing forward
        delay(300);
        long novaDistancia = medirDistancia();
        if (novaDistancia < 30 && novaDistancia > 0) {
            scanForClearPath();  // Rescan if obstacle detected
        }
    } else {  // Center (90°) is the clearest
        // No turn needed, check forward direction
        s.write(90);  // Ensure servo is facing forward
        delay(300);
        long novaDistancia = medirDistancia();
        if (novaDistancia < 30 && novaDistancia > 0) {
            scanForClearPath();  // Rescan if obstacle detected
        }
    }
}

void setup() {
    vel = 200;
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(buzzerPin, OUTPUT);  // Set buzzer pin as output
    Serial.begin(9600);           
    bluetooth.begin(9600);
    s.attach(pinoServo);
    s.write(90);  // Start with servo facing forward
}

void loop() {
    // Run sensor-based movement only if sensorAtivo is true
    if (sensorAtivo) {
        // Check distance using ultrasonic sensor
        long distancia = medirDistancia();
        
        // If obstacle is closer than 30cm
        if (distancia < 30 && distancia > 0) {
            parar();
            ativarBuzzer();  // Sound the buzzer only when obstacle is detected in front
            scanForClearPath();  // Scan and turn towards clearest path
        } else {
            irFrente(vel);  // Move forward if no obstacle
        }
    }

    // Comandos via Bluetooth
    if (bluetooth.available()) {
        char comando = bluetooth.read();

        if (comando == 'F') {
            irFrente(vel);
            bluetooth.println("Frente");
        } else if (comando == 'B') {
            irTras(vel);
            bluetooth.println("Re");
        } else if (comando == 'L') {
            virarEsquerda(vel);
            bluetooth.println("Esquerda");
        } else if (comando == 'R') {
            virarDireita(vel);
            bluetooth.println("Direita");
        } else if (comando == 'S') {
            parar();
            bluetooth.println("Parado");
        } else if (comando == 'X') {
            freiar();
            bluetooth.println("Freio");
        } else if (comando == 't') {
            sensorAtivo = !sensorAtivo;  // Toggle sensor mode
            if (!sensorAtivo) {
                parar();  // Stop the robot when sensor mode is disabled
            }
            bluetooth.println(sensorAtivo ? "Sensor Ativado" : "Sensor Desativado");
        } else if (comando == '1') {
            rotaSimples1();
            bluetooth.println("Rota1");
        } else if (comando == '2') {
            rotaSimples2();
            bluetooth.println("Rota2");
        } else if (comando == '3') {
            rotaSimples3();
            bluetooth.println("Rota3");
        } else if (comando == '4') {
            rotaSimples4();
            bluetooth.println("Rota4");
        } else if (comando == '5') {
            rotaSimples5();
            bluetooth.println("Rota5");
        }
    }
}
