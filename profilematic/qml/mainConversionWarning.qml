import QtQuick 1.0
import com.nokia.meego 1.0

PageStackWindow {
    id: root
    QueryDialog {
        id: dQuery
        titleText: "ProfileMatic update information"
        message: "Bla bla bla bla"
        onAccepted: Qt.quit();
        onRejected: Qt.quit();
    }

    Component.onCompleted: {
        dQuery.open();
    }
}
