# Temporary comment-outed for compilation
#compile_proto_to_cpp     (api.proto)
#compile_proto_to_grpc_cpp(api_service.proto)

compile_proto_to_cpp(block.proto)
compile_proto_to_cpp(primitive.proto)
compile_proto_to_cpp(commands.proto)
compile_proto_to_cpp(queries.proto)
compile_proto_to_cpp(responses.proto)
compile_proto_to_grpc_cpp(endpoint.proto)
compile_proto_to_grpc_cpp(peer_service.proto)

add_dependencies(commandsproto_h primitiveproto_h)
add_dependencies(queriesproto_h primitiveproto_h)
add_dependencies(blockproto_h commandsproto_h)
add_dependencies(endpointproto_h blockproto_h)

add_library(schema
    block.pb.cc
    commands.pb.cc
    primitive.pb.cc
    queries.pb.cc
)
target_link_libraries(schema
    protobuf
)

add_library(endpoint
    endpoint.pb.cc
    endpoint.grpc.pb.cc
)
target_link_libraries(endpoint
    protobuf
    grpc
    grpc++
    grpc++_reflection
)

add_library(peer_service_grpc
        peer_service.grpc.pb.cc
        peer_service.grpc.pb.h
        peer_service.pb.cc
        peer_service.pb.h
        )
target_link_libraries(peer_service_grpc
        protobuf
        grpc++
        )

