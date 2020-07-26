#ifndef TINYSAT_GRAPH_HPP_
#define TINYSAT_GRAPH_HPP_

#include <vector>
#include <stack>
#include <queue>
#include <algorithm>

namespace tinysat {

// Graph wrapper
//
// Interface:
// - dfs
// - bfs
// - topsort
// - condensation
//
// TODO:
// - to satisfy <boost/graph*> interface
//
template<template<typename> typename T>
class CGraph
{
public:
    using TEdgeSet = T<size_t>;
    using TEdgeSetIter = decltype(std::declval<TEdgeSet>().begin());

    //RAII - wrapper
    class CCleanWrapper
    {
    public:
        CCleanWrapper(CGraph* graph, bool cleanup);
        ~CCleanWrapper();

    private:
        CGraph* graph_;
        bool cleanup_;
    };

    explicit CGraph(size_t vert_cnt_set, size_t edge_cnt_set = 0u);

    template<typename TContainer>
    CGraph(size_t vert_cnt_set, size_t edge_cnt_set,
           const TContainer& edge_vec_set);

    CGraph             (const CGraph&) = default;
    CGraph& operator = (const CGraph&) = default;
    CGraph             (CGraph&&) = default;
    CGraph& operator = (CGraph&&) = default;

    [[nodiscard]] const auto& edge_vec() const noexcept
    {
        return edge_vec_;
    }

    void cleanup();

    void insert_bi_edge(size_t beg, size_t end);
    void insert_or_edge(size_t beg, size_t end);

    template<typename FOnEnter, typename FOnLeave, typename FOnBackEdge>
    bool 
    depth_first_unit(size_t start, FOnEnter&& on_enter, FOnLeave&& on_leave, 
                     FOnBackEdge&& on_back_edge, bool cleanup = true);

    template<typename FOnEnter, typename FOnLeave, typename FOnBackEdge, 
             typename FOnNextComp>
    bool 
    depth_first_comp(FOnEnter&& on_enter, FOnLeave&& on_leave, 
                     FOnBackEdge&& on_back_edge, FOnNextComp&& on_next_comp, 
                     bool cleanup = true);

    template<typename FOnEnter, typename FOnBackEdge>
    bool 
    breadth_first_unit(size_t start, FOnEnter&& on_enter, 
                       FOnBackEdge&& on_back_edge, bool cleanup = true);

    template<typename FOnEnter, typename FOnBackEdge, typename FOnNextComp>
    bool 
    breadth_first_comp(FOnEnter&& on_enter, FOnBackEdge&& on_back_edge, 
                       FOnNextComp&& on_next_comp, bool cleanup = true);

    std::vector<size_t> postorder();
    std::vector<size_t> preorder();
    std::vector<size_t> decompose();

    CGraph compress(const std::vector<size_t>& comp_vec);

private:
    size_t vert_cnt_, edge_cnt_;
    std::vector<bool> used_vec_;
    std::vector<TEdgeSet> edge_vec_;
    std::vector<TEdgeSetIter> next_it_vec_;
};

template<template<typename> typename T>
CGraph<T>::CCleanWrapper::CCleanWrapper(CGraph* graph, bool cleanup):
    graph_(graph), cleanup_(cleanup)
{}

template<template<typename> typename T>
CGraph<T>::CCleanWrapper::~CCleanWrapper()
{
    if (cleanup_) 
        graph_->cleanup();
}

template<template<typename> typename T>
CGraph<T>::
CGraph(size_t vert_cnt_set, size_t edge_cnt_set):
    vert_cnt_{ vert_cnt_set }, edge_cnt_{ edge_cnt_set },
    used_vec_(vert_cnt_), edge_vec_(vert_cnt_), next_it_vec_(vert_cnt_)
{}

template<template<typename> typename T>
template<typename TContainer>
CGraph<T>::
CGraph(size_t vert_cnt_set, size_t edge_cnt_set, 
       const TContainer& edge_vec_set):
    CGraph(vert_cnt_set, edge_cnt_set)
{
    for (size_t vert = 0u; vert != vert_cnt_; ++vert)
    {
        for (size_t next : edge_vec_set[vert])
            edge_vec_[vert].insert(edge_vec_[vert].end(), next);

        next_it_vec_[vert] = edge_vec_[vert].begin();
    }
}

template<template<typename> typename T>
void CGraph<T>::
cleanup()
{
    for (size_t vert = 0u; vert != vert_cnt_; ++vert)
    {
        used_vec_[vert] = false;
        next_it_vec_[vert] = edge_vec_[vert].begin();
    }
}

template<template<typename> typename T>
void CGraph<T>::
insert_bi_edge(size_t beg, size_t end)
{
    if (beg != end)
    {
        edge_vec_[beg].insert(edge_vec_[beg].end(), end);
        next_it_vec_[beg] = edge_vec_[beg].begin();

        edge_vec_[end].insert(edge_vec_[end].end(), beg);
        next_it_vec_[end] = edge_vec_[end].begin();
    }
    else
    {
        edge_vec_[beg].insert(edge_vec_[beg].end(), beg);
        next_it_vec_[beg] = edge_vec_[beg].begin();
    }
}

template<template<typename> typename T>
void CGraph<T>::
insert_or_edge(size_t beg, size_t end)
{
    edge_vec_[beg].insert(edge_vec_[beg].end(), end);
    next_it_vec_[beg] = edge_vec_[beg].begin();
}

template<template<typename> typename T>
template<typename FOnEnter, typename FOnLeave, typename FOnBackEdge>
bool CGraph<T>::
depth_first_unit(size_t start, FOnEnter&& on_enter, FOnLeave&& on_leave, 
                 FOnBackEdge&& on_back_edge, bool cleanup)
{
    CCleanWrapper cleanup_wrapper(this, cleanup);

    std::stack<size_t> dfs_stack;
    dfs_stack.push(start);
    used_vec_[start] = true;
    while (!dfs_stack.empty())
    {
        size_t root = dfs_stack.top();
        while (next_it_vec_[root] != edge_vec_[root].end() &&
               used_vec_[*next_it_vec_[root]])
        {
            if (!std::forward<FOnBackEdge>(on_back_edge)
                    (root, *next_it_vec_[root]))//<--------
                return false;

            ++next_it_vec_[root];
        }

        if (next_it_vec_[root] == edge_vec_[root].end())
        {
            dfs_stack.pop();
            if (!dfs_stack.empty())
            {
                if (!std::forward<FOnLeave>(on_leave)
                        (root, dfs_stack.top()))//<--------
                    return false;
            }
        }
        else
        {
            size_t next = *next_it_vec_[root];
            ++next_it_vec_[root];

            if (!std::forward<FOnEnter>(on_enter)
                    (next, root))//<--------
                return false;

            dfs_stack.push(next);
            used_vec_[next] = true;
        }
    }

    return true;
}

template<template<typename> typename T>
template<typename FOnEnter, typename FOnLeave, typename FOnBackEdge, 
         typename FOnNextComp>
bool CGraph<T>::
depth_first_comp(FOnEnter&& on_enter, FOnLeave&& on_leave, 
                 FOnBackEdge&& on_back_edge, FOnNextComp&& on_next_comp, 
                 bool cleanup)
{
    CCleanWrapper cleanup_wrapper(this, cleanup);

    size_t first_free = 0u;
    while (first_free != vert_cnt_)
    {
        if (!std::forward<FOnNextComp>(on_next_comp)
                (first_free))//<--------
            return false;

        // no forwarding because of the possible multiple calls
        if (!depth_first_unit(first_free, 
                              on_enter, on_leave, on_back_edge, 
                              false))
            return false;

        while (first_free != vert_cnt_ && used_vec_[first_free])
            ++first_free;
    }

    return true;
}

template<template<typename> typename T>
template<typename FOnEnter, typename FOnBackEdge>
bool CGraph<T>::
breadth_first_unit(size_t start, FOnEnter&& on_enter, 
                   FOnBackEdge&& on_back_edge, bool cleanup)
{
    CCleanWrapper cleanup_wrapper(this, cleanup);

    std::queue<size_t> bfs_queue;

    bfs_queue.push(start);
    used_vec_[start] = true;
    while (!bfs_queue.empty())
    {
        size_t root = bfs_queue.front();
        bfs_queue.pop();
        for (size_t next : edge_vec_[root])
        {
            if (!used_vec_[next])
            {
                bfs_queue.push(next);
                used_vec_[next] = true;

                if (!std::forward<FOnEnter>(on_enter)
                        (next, root))//<--------
                    return false;
            }
            else
            {
                if (!std::forward<FOnBackEdge>(on_back_edge)
                        (root, next))//<--------
                    return false;
            }
        }
    }

    return true;
}

template<template<typename> typename T>
template<typename FOnEnter, typename FOnBackEdge, typename FOnNextComp>
bool CGraph<T>::
breadth_first_comp(FOnEnter&& on_enter, FOnBackEdge&& on_back_edge, 
                   FOnNextComp&& on_next_comp, bool cleanup)
{
    CCleanWrapper cleanup_wrapper(this, cleanup);

    size_t first_free = 0u;
    while (first_free != vert_cnt_)
    {
        if (!std::forward<FOnNextComp>(on_next_comp)
                (first_free))//<--------
            return false;

        // no forwarding because of the possible multiple calls
        if (!breadth_first_unit(first_free, 
                                on_enter, on_back_edge, 
                                false))
            return false;


        while (first_free != vert_cnt_ && used_vec_[first_free])
            ++first_free;
    }

    return true;
}

template<template<typename> typename T>
std::vector<size_t> CGraph<T>::postorder()
{
    size_t prev_root = static_cast<size_t>(~0UL);

    std::vector<size_t> order_vec;
    order_vec.reserve(vert_cnt_);

    depth_first_comp(
        // on enter
        [] (size_t next, size_t root) { return true; },

        // on leave
        [&order_vec] (size_t vert, size_t root)
        {
            order_vec.push_back(vert);

            return true;
        },

        // on back edge
        [] (size_t root, size_t next) { return true; },

        // on next component
        [&order_vec, &prev_root] (size_t vert)
        { 
            if (prev_root != static_cast<size_t>(~0UL))
                order_vec.push_back(prev_root);

            prev_root = vert;

            return true; 
        }
    ); // depth_first_comp

    if (prev_root != static_cast<size_t>(~0UL))
        order_vec.push_back(prev_root);

    return order_vec;
}

template<template<typename> typename T>
std::vector<size_t> CGraph<T>::preorder()
{
    auto order_vec = postorder();
    std::reverse(std::begin(order_vec), std::end(order_vec));

    return order_vec;
}

template<template<typename> typename T>
std::vector<size_t> CGraph<T>::decompose()
{
    auto order_vec = preorder();

    CGraph inv_graph(vert_cnt_);
    for (size_t vert = 0u; vert < vert_cnt_; ++vert)
    {
        for (size_t next : edge_vec_[vert])
            inv_graph.insert_or_edge(next, vert);
    }

    size_t comp_idx = 0u;
    std::vector<size_t> comp_vec(vert_cnt_, vert_cnt_);

    for (size_t vert_idx : order_vec)
    {
        if (comp_vec[vert_idx] != vert_cnt_)
            continue;

        comp_vec[vert_idx] = comp_idx;
        inv_graph.depth_first_unit(
            //start vertex
            vert_idx,
            //on enter
            [&comp_vec, &comp_idx] 
            (size_t next, size_t root)
            { 
                comp_vec[next] = comp_idx;
                return true; 
            },
            //on leave
            [] (size_t vert, size_t root) { return true; },
            //on back edge
            [] (size_t root, size_t next) { return true; },
            //clean
            false
        );//graph.breadth_first_unit

        ++comp_idx;
    }

    return comp_vec;
}

template<template<typename> typename T>
CGraph<T> CGraph<T>::compress(const std::vector<size_t>& comp_vec)
{
    auto max_it = std::max_element(std::begin(comp_vec), std::end(comp_vec));
    size_t comp_cnt = *max_it + 1u;

    CGraph result(comp_cnt);
    for (size_t vert = 0u; vert < vert_cnt_; ++vert)
    {
        for (size_t next : edge_vec_[vert])
        {
            result.edge_vec_[comp_vec[vert]]
                .insert(result.edge_vec_[comp_vec[vert]].end(), comp_vec[next]);
        }
    }

    return result;
}

} // namespace tinysat

#endif // TINYSAT_GRAPH_HPP_
