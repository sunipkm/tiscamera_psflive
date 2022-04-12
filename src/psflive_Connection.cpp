#include "psflive_Connection.h"
#include "psflive_function.h"
// ====================================================================================================================










// ====================================================================================================================
psflive::Connection::Connection(const std::string& _address, const ushort& _port) : m_address(_address), m_port(_port), m_socket(CONTEXT, zmq::socket_type::pair), m_listening(false) {
  m_socket.set(zmq::sockopt::routing_id, PSFLIVE_CHANNEL);
  m_socket.set(zmq::sockopt::linger, 10);
  m_socket.bind(getAddressPort().c_str());
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
zmq::send_result_t psflive::Connection::Send(const std::string& _message) {
  psflive::function::LogMessage("Connection","Send()","message = "+_message);
  zmq::message_t message(_message);
  return m_socket.send(message, zmq::send_flags::dontwait); // send init, does not block
}
// --------------------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------------------
std::string psflive::Connection::Receive() {
  zmq::message_t recvMessage;
  m_socket.recv(recvMessage, zmq::recv_flags::dontwait); // receive string data
  return recvMessage.to_string();
}
// --------------------------------------------------------------------------------------------------------------------