// M5Stackで温湿度センサーDHT22(AM2302)を使用するサンプル
// SW参考:
// https://www.instructables.com/id/ESP32-M5Stack-With-DHT22/

// DHT22の結線は以下の通り
//    ピンはセンサー正面向かって左から
//    1 - VDD(3.3V)
//    2 - SCA(GPIO5[M5Stack本体正面向かって右側上から6番目])
//    3 - NC(nouse)
//    4 - GND(GND)
//    ※1と2を抵抗10k(茶黒橙金)でつなぐ
// HW参考:
// https://littlewing.hatenablog.com/entry/2016/06/09/140637


// M5StackとDHTのライブラリ
#include <M5Stack.h>
#include <SimpleDHT.h>

// 色の定義
#define BLACK   0x0000
#define RED     0xF800
#define CYAN    0x07FF
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define DHTPIN 2 // DHTで使用するピンNo

// センサーオブジェクト
SimpleDHT22 dht;

int leituraAtual = 1;

// X軸とY軸の定義
#define POS_X_GRAFICO       30
#define POS_Y_GRAFICO       3
#define ALTURA_GRAFICO      180
#define COMPRIMENTO_GRAFICO 270

// 温度と湿度のデータを書き込む座標の定義
#define POS_X_DADOS 30
#define POS_Y_DADOS 200

// 湿度と温度の読み取り値を保存する変数
int umidade = 0;
int temperatura = 0;

// 例の線を描くためにy座標の値を保存する変数
// 値は1から1に変化します
int linhaExemplo = 20;
int fator = 1; // 行変数で単位を加算するか減算するかを示します



void setup() {
  Serial.begin(115200);

  // M5Stackの初期化
  M5.begin();

  // 画面全体を黒く塗る
  M5.Lcd.fillScreen(BLACK);

  // 次のコマンドは、デカルト軸線を白で描画します
  // drawFastVLine(x, y, width, color) --> 縦線
  M5.Lcd.drawFastVLine(POS_X_GRAFICO, POS_Y_GRAFICO, ALTURA_GRAFICO, WHITE); // Y軸
  // drawFastHLine(x, y, width, color) --> 横線
  M5.Lcd.drawFastHLine(POS_X_GRAFICO, ALTURA_GRAFICO+1, COMPRIMENTO_GRAFICO, WHITE); // X軸

  // 画面に書き込むテキストのサイズを設定します
  M5.Lcd.setTextSize(3);
  // テキストに白色を設定します
  M5.Lcd.setTextColor(WHITE);

  // 書き込み用のカーソルの位置
  M5.Lcd.setCursor(POS_X_DADOS, POS_Y_DADOS);
  M5.Lcd.print("T: "); // 温度を示す
  M5.Lcd.setCursor(POS_X_DADOS+105, POS_Y_DADOS);
  M5.Lcd.print(" U: "); // 湿度を示す

}

void loop() {
  // 温度と湿度を読み取ります
  float temp, umid;
  int status = dht.read2(DHTPIN, &temp, &umid, NULL);

  if (status == SimpleDHTErrSuccess) {
    temperatura = temp;
    umidade = umid;
  }

  // 変数の値をプロットにマッピングする
  // ディスプレイの高さが240pxであり、チャートでは180だけを分離したため、必要です。
  // 湿度は0-100から読むことができます
  int temperaturaMapeada = map(temperatura, 0, 100, 0, ALTURA_GRAFICO);
  int umidadeMapeada = map(umidade, 0, 100, 0, ALTURA_GRAFICO);

  // センサーの読み取り値を参照する点を画面に描画します
  M5.Lcd.drawPixel(POS_X_GRAFICO+leituraAtual, ALTURA_GRAFICO-temperaturaMapeada, RED);
  M5.Lcd.drawPixel(POS_X_GRAFICO+leituraAtual, ALTURA_GRAFICO-umidadeMapeada, CYAN);

  // 変化しているサンプル行を参照する点を画面に描画します
  M5.Lcd.drawPixel(POS_X_GRAFICO+leituraAtual, ALTURA_GRAFICO-linhaExemplo, YELLOW);

  // ここで、サンプル行を制御します。最大値に達したら、値を減らします
  // 特定の最小値（この場合は10）まで、その後再び増加
  if (linhaExemplo == 70) fator = -1;
  else if (linhaExemplo == 10) fator = 1;

  // 行の値を合計する
  linhaExemplo += fator;

  // 読み取りカウンタをインクリメントします
  leituraAtual++;
  
  // 読み取り値が270（最大x軸番号）に達した場合、グラフの領域をクリアして再度描画します。
  if (leituraAtual == 270)
  {
    // チャートの全領域をクリアします
    M5.Lcd.fillRect(POS_X_GRAFICO+1, POS_Y_GRAFICO-1, COMPRIMENTO_GRAFICO, ALTURA_GRAFICO-1, BLACK);
    leituraAtual = 1; // 読み取りカウンターを1に設定します（新しいX座標）
  }

  // 温度と湿度の値を設定した領域をクリアします
  M5.Lcd.fillRect(POS_X_DADOS+50, POS_Y_DADOS, 60, 30, BLACK);
  M5.Lcd.fillRect(POS_X_DADOS+165, POS_Y_DADOS, 90, 30, BLACK);

  // カーソルを再配置して温度を書き込む
  M5.Lcd.setCursor(POS_X_DADOS+50, POS_Y_DADOS);

  M5.Lcd.setTextColor(RED);
  M5.Lcd.print(temperatura);
  M5.Lcd.print((char)247);

  // 水分を書き込むためにカーソルの位置を変更します
  M5.Lcd.setCursor(POS_X_DADOS+165, POS_Y_DADOS);

  M5.Lcd.setTextColor(CYAN);
  M5.Lcd.print(umidade);
  M5.Lcd.print("%");

  delay(1000);
}
