#include "ttopology.h"
#include <algorithm>
#include <sstream>

bool TTopology::AddNode(int id, int parent_id, const std::string& endpoint) {
    if (nodes_.find(id) != nodes_.end()) return false; // Узел уже существует

    // Проверяем, что родительский узел существует (если parent_id != -1)
    if (parent_id != -1 && nodes_.find(parent_id) == nodes_.end()) {
        return false; // Родительский узел не найден
    }

    TNodeInfo info;
    info.id = id;
    info.parent_id = parent_id;
    info.endpoint = endpoint;
    info.alive = true;
    info.last_heartbeat = std::chrono::steady_clock::now();

    // Добавляем узел в список
    nodes_[id] = info;

    // Если у узла есть родитель, добавляем его в список дочерних узлов родителя
    if (parent_id != -1) {
        nodes_[parent_id].children.push_back(id);
    }

    return true;
}

bool TTopology::RemoveNode(int id) {
    if (nodes_.find(id) == nodes_.end()) return false; // Узел не найден

    // Рекурсивно удаляем поддерево
    RemoveSubtree(id);
    return true;
}

void TTopology::RemoveSubtree(int id) {
    // Рекурсивно удаляем всех потомков
    for (int child_id : nodes_[id].children) {
        RemoveSubtree(child_id);
    }

    // Удаляем узел из списка дочерних узлов родителя (если есть родитель)
    if (nodes_[id].parent_id != -1) {
        auto& parent_children = nodes_[nodes_[id].parent_id].children;
        parent_children.erase(std::remove(parent_children.begin(), parent_children.end(), id), parent_children.end());
    }

    // Удаляем узел из основного списка
    nodes_.erase(id);
}

TNodeInfo* TTopology::GetNode(int id) {
    auto it = nodes_.find(id);
    if (it == nodes_.end()) return nullptr;
    return &it->second;
}

bool TTopology::NodeExists(int id) {
    return nodes_.find(id) != nodes_.end();
}

std::vector<int> TTopology::GetChildren(int id) {
    std::vector<int> children;
    if (id == -1) {
        // Возвращаем все корневые узлы (узлы без родителя)
        for (const auto& [node_id, info] : nodes_) {
            if (info.parent_id == -1) {
                children.push_back(node_id);
            }
        }
    } else if (nodes_.find(id) != nodes_.end()) {
        // Возвращаем дочерние узлы для конкретного узла
        children = nodes_[id].children;
    }
    return children;
}

// Реализация метода PingAll
std::string TTopology::PingAll() {
    std::vector<int> unavailable_nodes;
    for (const auto& [id, info] : nodes_) {
        if (!info.alive) {
            unavailable_nodes.push_back(id);
        }
    }

    if (unavailable_nodes.empty()) {
        return "-1"; // Все узлы доступны
    }

    std::ostringstream oss;
    for (size_t i = 0; i < unavailable_nodes.size(); ++i) {
        oss << unavailable_nodes[i];
        if (i < unavailable_nodes.size() - 1) {
            oss << ";";
        }
    }

    return oss.str(); // Возвращаем строку с недоступными узлами
}