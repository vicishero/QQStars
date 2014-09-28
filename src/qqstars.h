#ifndef QQCommand_H
#define QQCommand_H

#include <QObject>
#include <QDebug>
#include <QByteArray>
#include <QTimer>
#include "mymessagebox.h"
#include "mynetworkaccessmanagerfactory.h"

class QQCommand : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString userQQ READ userQQ WRITE setUserQQ NOTIFY userQQChanged)
    Q_PROPERTY(QString userPassword READ userPassword WRITE setUserPassword NOTIFY userPasswordChanged)
    Q_PROPERTY(QQStatus userStatus READ userStatus WRITE setUserStatus NOTIFY userStatusChanged)
    Q_PROPERTY(QString userStatusToString READ userStatusToString NOTIFY userStatusToStringChanged)
    Q_PROPERTY(LoginStatus loginStatus READ loginStatus WRITE setLoginStatus NOTIFY loginStatusChanged)
    Q_PROPERTY(double windowScale READ windowScale WRITE setWindowScale NOTIFY windowScaleChanged)
    Q_ENUMS(QQStatus)
    Q_ENUMS(LoginStatus)
    Q_ENUMS(SenderType)
    Q_ENUMS(MessageType)
    Q_ENUMS(ProxyType)
   
public:
    explicit QQCommand(QQuickItem *parent = 0);
    enum LoginStatus{
        Offline,//离线
        Logining,//登录中
        LoginFinished//登录完成
    };

    enum QQStatus{
        Offlineing,//离线
        Online,//在线
        Callme,//Q我吧
        Away,//离开
        Busy,//忙碌
        Silent,//请勿打扰
        Hidden//隐身
    };
    enum SenderType{
        Friend,//好友
        Group,//群
        Discu,//讨论组
        Stranger,//陌生人
        SystemMessage//系统消息，包含群管理员的更改，成员的更改，还有好友验证消息
    };
    
    enum MessageType{
        InputNotify,//正在输入
        Text,//文本
        Image,//图片
        Face,//表情
        GeneralMessage,//普通消息，包含Text Image Face等
        //SendFile,//发送文件
        //CancleSendFile,//取消发送文件
        FileMessage,//文件消息，包含发送文件和取消发送文件
        AvRequest,//请求开视频
        AvRefuse,//取消开视频
        ShakeWindow,//窗口抖动
        FriendStatusChanged,//好友状态改变
        FriendVerify,//好友验证消息poll_type=system_message，type=verify_required
        GroupAdmin,//群管理员消息poll_type=sys_g_msg，type=group_admin_op，uin_flag=1为设为管理员，0为取消管理员
        GroupLeave//群T人的消息
    };
    
    QString userStatusToString() const
    {
        return m_userStatusToString;
    }
    LoginStatus loginStatus() const
    {
        return m_loginStatus;
    }
    
    QString userQQ() const
    {
        return m_userQQ;
    }
    
    QString userPassword() const
    {
        return m_userPassword;
    }
    
    double windowScale() const
    {
        return m_windowScale;
    }
    
private slots:
    void beginPoll2();
    void poll2Finished(QNetworkReply *replys);
private:
    QQStatus qq_status;
    QQStatus userStatus();
    void setUserStatus( QQStatus new_status );

    QString m_userStatusToString;
    LoginStatus m_loginStatus;

    QByteArray poll2_data;//post心跳包的数据
    
    NetworkAccessManager manager;
    QNetworkRequest request;
    QString m_userQQ;
    QString m_userPassword;

    QJSEngine jsEngine;
    void loadApi();
    
    QString analysisMessage( QJsonObject &obj );//解析基本消息
    //void disposeInputNotify( QJsonObject &obj );//处理好友正在输入消息
    void disposeFriendStatusChanged( QJsonObject &obj );//处理好友状态改变
    void disposeFriendMessage( QJsonObject &obj, MessageType type=GeneralMessage );//处理好友消息
    void disposeGroupMessage( QJsonObject &obj, MessageType type=GeneralMessage );//处理群消息
    void disposeDiscuMessage( QJsonObject &obj, MessageType type=GeneralMessage );//处理讨论组消息
    void disposeStrangerMessage( QJsonObject &obj, MessageType type=GeneralMessage );//处理陌生人消息
    void disposeSystemMessage(QJsonObject &obj);//处理系统消息
    //void disposeFileMessage( QJsonObject &obj );//处理文件传输方面的消息
    //void disposeAvMessage( QJsonObject &obj, bool open/*true为开视频，false为取消开视频*/ );//处理视频聊天方面的消息
    //void disposeShakeMessage( QJsonObject &obj );
    
    QString doubleToString( QJsonObject &obj, QString name );//将obj中类型为double的数据转化为QString类型
    
    double m_windowScale;

signals:
    void userStatusChanged();
    void userStatusToStringChanged();
    void loginStatusChanged(LoginStatus arg);
    void poll2ReData( QString data );
    void userQQChanged(QString arg);
    void error( QString message );//有错误产生就发送信号
    void updateCode();//刷新验证码
    void inputCodeClose();//关闭验证码输入
    void userPasswordChanged(QString arg);
    
    void windowScaleChanged(double arg);
    void messageArrive(SenderType senderType, QString uin, QString jsonData);
public slots:
    void setLoginStatus(LoginStatus arg);
    void startPoll2( QByteArray data );
    void setUserQQ(QString arg)
    {
        if (m_userQQ != arg) {
            m_userQQ = arg;
            
            emit userQQChanged(arg);
        }
    }
    void setUserPassword(QString arg)
    {
        if (m_userPassword != arg) {
            m_userPassword = arg;
            emit userPasswordChanged(arg);
        }
    }
    void showWarningInfo(QString message);
    void downloadImage( QUrl url, QString uin, QString imageSize, QJSValue callbackFun );
    
    void setValue(const QString & key, const QVariant & value, const QString & userQQ="");
    QVariant getValue(const QString & key, const QVariant & defaultValue = QVariant(), const QString & userQQ="") const;
    void removeValue( const QString & key, const QString & userQQ="" );
    
    void updataApi(const QString content);
    QString getHash();
    QString encryptionPassword(const QString &uin, const QString &code);
    void setWindowScale(double arg)
    {
        if (m_windowScale != arg) {
            m_windowScale = arg;
            emit windowScaleChanged(arg);
        }
    }
    
    int openMessageBox( QJSValue value );
};

#endif // QQCommand_H
