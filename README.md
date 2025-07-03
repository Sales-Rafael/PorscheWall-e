🤖 Robô Controlado por Bluetooth com Sensor Ultrassônico e Servo Motor
Este projeto Arduino implementa um robô móvel que pode ser controlado via Bluetooth ou por meio de um modo automático com desvio de obstáculos, utilizando um sensor ultrassônico, servo motor, motores DC, e buzzer para alerta sonoro. Ideal para aprendizado de robótica com Arduino e sensores.

📦 Funcionalidades
Controle manual via Bluetooth com comandos:

F – Frente

B – Ré (Trás)

L – Virar à esquerda

R – Virar à direita

S – Parar

X – Frear

Modo automático com desvio de obstáculos

Alerta sonoro com buzzer ao detectar obstáculo

Execução de rotas pré-programadas:

Rota 1: Movimento em padrões de curva

Rota 2: Movimento com curva suave

Rota 3: Movimento em zigue-zague

Rota 4: Rota com rotações de 360°

Rota 5: Movimento curto e repetido

🧠 Componentes Utilizados
Arduino Uno ou compatível

Módulo Bluetooth (ex: HC-05)

Módulo Sensor Ultrassônico HC-SR04

Servo Motor (ex: SG90)

4 Motores DC com Ponte H (L298N ou similar)

Buzzer

Jumpers e Fonte de Alimentação

📷 Diagrama de Conexões (Pinagem)
Componente	Pino Arduino
Bluetooth RX	10
Bluetooth TX	11
Servo Motor	12
Sensor Trig	9
Sensor Echo	2
Motor Esquerdo	
• P1	7
• P2	8
• Velocidade	6
Motor Direito	
• P1	4
• P2	5
• Velocidade	3
Buzzer	A0

🚀 Como Usar
Grave o código no seu Arduino utilizando a IDE do Arduino.

Conecte o módulo Bluetooth ao seu celular.

Use um aplicativo Bluetooth Serial (ex: Bluetooth Electronics, Arduino Bluetooth Controller) e envie os comandos desejados.

Para ativar ou desativar o modo automático com sensor ultrassônico, envie o comando t.

Para executar rotas automáticas, envie os comandos de 1 a 5.

🧠 Lógica do Modo Automático
O robô move-se para frente enquanto não encontra obstáculos.

Ao detectar um obstáculo a menos de 30cm:

Ele para, ativa o buzzer e faz uma varredura com o servo.

Gira o sensor de 0° a 180° para identificar o caminho mais livre.

Escolhe a direção com mais espaço e executa a curva apropriada.

Retorna ao movimento para frente se a frente estiver livre.
