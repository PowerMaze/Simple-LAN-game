#pragma once
/*
///////////////////////////////////////////////////
classes pre declarations 
///////////////////////////////////////////////////
*/

class CBrush;   // Кисть, випуклий статичний об'єкт
class CConsole; // Консоль, де зберігаються логи виведені через функцію PrintF
class CConsoleDataBase; // База даних консолі, оболонка для виконання команд
class CDataBase;   // База даних дял завантаження об'єктів
class CEntityMain; // Головна ігрова сутність
class CFileName; // Клас для роботи з рядками типу файлів
class CFileOBJ;  // Імпорт моделей у форматі Wavefront (OBJ)
class CFileStream; // Клас для операції з файлами
class CFont;       // Шрифт для виведення на екран
class CGameWorld;  // Ігровий світ
class CModel;           // Екземпляр 3D моделі
class CModelAnimFrames; // Робота з анімаціями
class CModelObject; // полігональна 3D модель
class COBJDecoder;  // Імпорт моделей, старіша версія
class CPhysicsEntity; // Сутність з колізіями та фізикою
class CPolygonWorld;  // Полігон дял випуклих та опуклих об'єктів
class CRoomBase; // Сектор, або опуклий об'єкт
class CShader;   // Шейдер для OpenGL
class CSkyBox;   // Небо
class CTexture;  // Екземпляр растового файлу
class CTextureObject; // Растовий файл для використання текстур
class CVertex; // Вершина для об'єктів
class CView;   // Головний екран гри