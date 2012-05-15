//
// server.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_RPC_HTTP_SERVER_HPP
#define BOOST_RPC_HTTP_SERVER_HPP

#include <boost/asio.hpp>
#include <string>
#include <boost/array.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/rpc/http/coroutine.hpp>
#include <boost/rpc/http/request_parser.hpp>

namespace boost { namespace rpc { namespace http {

  struct request;
  struct reply;

    /// The top-level coroutine of the HTTP server.
  class server : coroutine {
    public:
      /// Construct the server to listen on the specified TCP address and port, and
      /// serve up files from the given directory.
      explicit server( const std::string& address, const std::string& port,
          boost::function<void(const request&, reply&)> request_handler);

      /// Perform work associated with the server.
      void operator()(
          boost::system::error_code ec = boost::system::error_code(),
          std::size_t length = 0);

    private:
      typedef boost::asio::ip::tcp tcp;

      /// The user-supplied handler for all incoming requests.
      boost::function<void(const request&, reply&)> request_handler_;

      /// Acceptor used to listen for incoming connections.
      boost::shared_ptr<tcp::acceptor> acceptor_;

      /// The current connection from a client.
      boost::shared_ptr<tcp::socket> socket_;

      /// Buffer for incoming data.
      boost::shared_ptr<boost::array<char, 8192> > buffer_;

      /// The incoming request.
      boost::shared_ptr<request> request_;

      /// Whether the request is valid or not.
      boost::tribool valid_request_;

      /// The parser for the incoming request.
      request_parser request_parser_;

      /// The reply to be sent back to the client.
      boost::shared_ptr<reply> reply_;

      boost::shared_ptr<boost::asio::strand>  strand_;
      boost::function<void(boost::system::error_code,std::size_t)> this_strand;
  };

} } }// boost::rpc::http

#endif // HTTP_SERVER4_SERVER_HPP