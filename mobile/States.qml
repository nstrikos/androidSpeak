import QtQuick 2.0

import QtQuick 2.15
import QtQuick.Window 2.15

import QtQml.StateMachine 1.15 as DSM

Item {
    DSM.StateMachine {
        id: stateMachine
        initialState: waitingState
        running: true

        DSM.State {
            id: waitingState
            DSM.SignalTransition {
                targetState: showInfoState
                signal: window.connectionEstablished
            }
            DSM.SignalTransition {
                targetState: settingsState
                signal: window.showSettings
            }
            onEntered: window.waitingStateEntered()
        }

        DSM.State {
            id: settingsState
            DSM.SignalTransition {
                targetState: waitingState
                signal: window.exitSettings
                guard: !window.isConnected
            }
            DSM.SignalTransition {
                targetState: showInfoState
                signal: window.exitSettings
                guard: window.isConnected
            }
            onEntered: window.settingsStateEntered()
            onExited: window.settingsStateExited()
        }

        DSM.State {
            id: showInfoState
            DSM.SignalTransition {
                targetState: waitingState
                signal: window.disconnected
            }
            DSM.SignalTransition {
                targetState: connectedState
                signal: window.countDownFinished
            }
            DSM.SignalTransition {
                targetState: settingsState
                signal: window.showSettings
            }
            onEntered: window.showInfoStateEntered()
            onExited: window.showInfoStateExited()
        }

        DSM.State {
            id: connectedState
            DSM.SignalTransition {
                targetState: waitingState
                signal: window.disconnected
            }
            DSM.SignalTransition {
                targetState: showInfoState
                signal: window.wake
            }
            onEntered: window.connectedStateEntered()
        }
    }
}

