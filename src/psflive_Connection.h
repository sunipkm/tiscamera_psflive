#ifndef __PSFLIVE_CONNECTION_H__
#define __PSFLIVE_CONNECTION_H__

#pragma once

#include <thread>
#include <atomic>
#include <memory>
#include <zmq.hpp>
#include <zmq_addon.hpp>

#include "psflive_def.h"
// ====================================================================================================================










// ====================================================================================================================
namespace psflive {
  class Connection {
    static zmq::context_t CONTEXT; // global
  private:
    std::string m_address;
    ushort m_port;
    zmq::socket_t m_socket;
    std::atomic<bool> m_listening;
  public:
    std::thread m_threadId;
    // ---- constructor -----------------------------------------------------------------------------------------------
    Connection(const std::string& _address=PSFLIVE_DEFAULT_ADDRESS, const ushort& _port=PSFLIVE_DEFAULT_PORT);
    // ---- destructor ------------------------------------------------------------------------------------------------
    ~Connection() = default;
    // ----------------------------------------------------------------------------------------------------------------
    zmq::send_result_t Send(const std::string& _message);
    std::string Receive();
    // ----------------------------------------------------------------------------------------------------------------
    std::string getAddressPort() const {
      return m_address+":"+std::to_string(m_port);
    }
    // ----------------------------------------------------------------------------------------------------------------
    void setAddress(const std::string& _address) {
      m_address = _address;
    }
    std::string getAddres() const {
      return m_address;
    }
    // ----------------------------------------------------------------------------------------------------------------
    void setPort(const ushort& _port) {
      m_port = _port;
    }
    ushort getPort() const {
      return m_port;
    }
    // ----------------------------------------------------------------------------------------------------------------
    void setListen(bool _listen=true) {
      m_listening = _listen;
    }
    bool isListening() const {
      return m_listening;
    }
    // ----------------------------------------------------------------------------------------------------------------
  };
};
// ====================================================================================================================
#endif //__PSFLIVE_CONNECTION_H__
